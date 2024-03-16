import matplotlib.pyplot as plt
from numpy import ndarray
import pandas as pd
import numpy as np
from sklearn.model_selection import GridSearchCV, train_test_split
from sklearn.preprocessing import OneHotEncoder, StandardScaler, RobustScaler, QuantileTransformer, FunctionTransformer
from sklearn.impute import SimpleImputer
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline, make_pipeline
from sklearn.compose import ColumnTransformer
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.pipeline import make_pipeline
from sklearn.feature_extraction import FeatureHasher
from sklearn.ensemble import IsolationForest
from sklearn.preprocessing import LabelEncoder
import seaborn as sns
from sklearn.metrics import precision_score, recall_score, accuracy_score, f1_score
from table import table
from sklearn.svm import SVC
# Add any other imports that you need here

# If you created custom transformers or helper functions, you can also add them to this file.
class MunselColorMapper(BaseEstimator, TransformerMixin):
    """This transformer maps the Munsel color to three columns."""
    def fit(self, X, y=None):
        return self  # nothing to fit

    def transform(self, X):
        new_cols = X['munsel_color'].apply(lambda x: pd.Series(table[x], index=['munsel_color_1', 'munsel_color_2', 'munsel_color_3']))
        X_mapped = pd.concat([X, new_cols], axis=1)
        X = X_mapped.drop('munsel_color', axis=1)

        return X
    
class DropDuplicatesTransformer(BaseEstimator, TransformerMixin):
    """This transformer drops duplicate rows."""
    
    def __init__(self):
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X):
        return X.drop_duplicates().copy()

class OutlierRemover(BaseEstimator, TransformerMixin):
    """This transformer removes outliers using an IsolationForest."""
    
    def __init__(self, outlier_detector):
        self.outlier_detector = outlier_detector

    def fit(self, X, y=None):
        self.outlier_detector.fit(X)
        return self

    def transform(self, X, y=None):
        is_inlier = self.outlier_detector.predict(X) > 0 # 1 for inliers, -1 for outliers
        return X[is_inlier]
    
class LogTransformer(BaseEstimator, TransformerMixin):
    """This class does a simple log-transform of some of the data."""

    def __init__(self, seed = 1e-5):
        self.seed=seed
        return None
    
    def fit(self, X, y=None):
        return self
    
    def transform(self, X, y=None):
        return np.log(X+self.seed)

class ZeroTransform(BaseEstimator, TransformerMixin):
    """This transformer replaces negative values by zeros: elemental concentrations cannot be < zero."""
    
    def fit(self, X, y=None):
        return self
    
    def transform(self, X, y=None):
        X = np.where(X < 0, 0, X)
        return X

class LithoEstimator:
    '''Used to predict lithology in IODP wells. The signature (method name, argument and return types) for the strict minimum number of methods needed are already written for you below.
    Simply complete the methods following your own notebook results. You can also add more methods than provided below in order to keep your code clean.'''

    def __init__(self, path:str='data/log_data.csv') -> None:
        '''The path is a path to the training file. The default is the file I gave you.
        You want to create an X_train, X_test, y_train and y_test following the same principle as in your
        notebook. You also want to define and train your estimator as soon as you create it.
        
        I recommend creatubg the following instance variables in your __init__ method:
        self.X_train, self.X_test, self.y_train, self.y_test
        self.encoder - the label encoder for your categories
        self.model - the entire trained model pipeline

        Note that this class should not handle hyperparameter searching or feature selection - if you did those in your Part B 
        simply use your best estimators.
        
        '''
        self.data_raw = pd.read_csv(path) # This is your EDA (Part A) and training data (Parts B and C)
        self.data_preprocessed = None # This is your preprocessed data (Part B)
        self.X_train, self.X_test, self.y_train, self.y_test = self.load_and_split(path)
        self.model = None

    def load_and_split(self, path:str='data/log_data.csv'):
        """Loads the data and splits it into train and test sets. Returns X_train, X_test, y_train, y_test."""
        self.data_raw = pd.read_csv(path) 
        self.data_preprocessed = self.data_raw.drop_duplicates()
        y = self.data_preprocessed['lithology']
        self.le = LabelEncoder()
        self.le.fit(y)
        y = self.le.transform(y) # y is now a numpy array of integers
        self.data_preprocessed['lithology'] = y

        test_frac = int(self.data_preprocessed.shape[0]*.3)+1
        sorted_data = self.data_preprocessed.sort_values(by=['DEPTH_WMSF'], ascending=False)        
        test_set = sorted_data.iloc[:test_frac]
        train_set = sorted_data.iloc[test_frac:]
        
        X_train, y_train = self.get_X_y(train_set)
        X_test, y_test = self.get_X_y(train_set)
        return X_train, X_test, y_train, y_test
    
    def create_model(self):
        """Creates and trains the model pipeline."""
        data_pipe = self.data_pipe()
        # self.model = make_pipeline(data_pipe, LogisticRegression(max_iter=5000))
        self.model = make_pipeline(data_pipe, SVC(kernel="linear", C=100, gamma=1))
        self.model.fit(self.X_train, self.y_train)
        # return self.model
        return None
    
    def data_pipe(self) -> pd.DataFrame:
        """Returns the data pipeline."""
        num_features = ['HCGR','HURA']
        num_log_features = ['IMPH', 'SFLU']
        cat_features = ['munsel_color']

        num_pipe = make_pipeline(SimpleImputer(), ZeroTransform(), StandardScaler()) 
        num_log_pipe = make_pipeline(SimpleImputer(), ZeroTransform(), LogTransformer(), StandardScaler())
        cat_pipe = make_pipeline(MunselColorMapper(), StandardScaler())

        full_pipe = ColumnTransformer([
            ('num_pipe', num_pipe, num_features),
            ('num_log_pipe', num_log_pipe, num_log_features),                           
            ('cat_pipe', cat_pipe, cat_features)])
        
        return full_pipe
        
    def get_X_y(self, df):
        """Returns X and y."""
        X = df.drop(['lithology'], axis=1)
        y = df['lithology']
        return X, y
    
    def x_test_score(self) -> np.float:
        '''Returns the F1 macro score of the X_test. This should be of type float.'''
        return f1_score(self.y_test, self.model.predict(self.X_test), average='macro')

    def get_Xs(self) -> (pd.DataFrame, pd.DataFrame):
        '''Returns the X_train and X_test. This method is already written for you.'''

        return self.X_train, self.X_test
    
    def get_ys(self) -> (pd.DataFrame, pd.DataFrame):
        '''Returns the y_train and y_test. This method is already written for you.'''

        return self.y_train, self.y_test

    def predict(self, path_to_new_file:str='data/new_data.csv') -> np.array:
        '''Uses the trained algorithm to predict and return the predicted labels on an unseen file.
        The default file is the unknown_data.csv file in your data folder.
        
        Return a numpy array (the default for the "predict()" function of sklearn estimator)'''
        
        new_data = pd.read_csv(path_to_new_file)
        new_data.drop_duplicates(inplace=True)
        # datapipe = self.data_pipe()
        # new_data = datapipe.transform(new_data)
        results = self.le.inverse_transform(self.model.predict(new_data))
        return results

    def get_model(self) -> Pipeline:
        '''returns the entire trained pipeline, i.e. your model. 
        This will include the data preprocessor and the final estimator.'''

        return self.model

