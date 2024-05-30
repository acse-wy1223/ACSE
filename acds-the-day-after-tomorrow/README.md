# The Day After Tomorrow

## Predicting tropical storm behaviour through Deep Learning

FEMA (Federal Emergency Management Agency) in the US have released an open competition to improve their emergency protocols under hurricane threats.
Team Nicholas is responding to the call for ML specialists that can provide a solution to the problem of forecasting the evolution of tropical cyclones in real-time.
Team Nicholas has been working on the design and implementation of a solution and a presentation for the FEMA panel of experts has been saved in the Microsoft Teams Channel for Team Nicholas.

## Contents of this README
- Github repo folder structure
- Task 1 - Image Generation
- Task 2 - Wind Speed
- User instructions
- Libraries required
- Original links to the training dataset, surprise storm and briefing slides

### Github repo folder structure
This repo, "acds-the-day-after-tomorrow-nicholas", contains the folders:
- "ACDS"
    - "Image" - contains Task 1 workbooks§
    - "Speed" - contains Task 2 workbooks
- "Data"
    - "Training data" - contains initial training data from 30 storms.
    - "Surprise Storm tst"
    - "Results" - contains results for task 1 & 2, the 3 images and csv containing 
- "EDA/Creativity" - contains Exploratory data analysis and attempts for various models

### Task 1 - Image Generation
In Task 1, Team Nicholas was given data from one storm (Surprise Storm tst), where some satellite images are available, to generate a ML/DL-based solution able to generate 3 future image predictions based on these existing images for that given storm.

The 3 images generated from Task 1 are saved under "Data" -> "results" folder.

#### Task 1 Models
In Task1, Team Nicholas has tried many models throughout the week. 
- In the first, we have tried a **CNN+LSTM** model to predict the future images by previous images and other features. But it didn't work well, the picture is not clear enough.
- We have also tried to use only **Resnet+LSTM** structure to predict the future state. However, the picture we generated wouldn't change along with the change of relative time.
- The final model we use is a **Encoder+Convolutional LSTM+Decoder** model. It effectively saves the time series information, but the quality of the images were not sufficient. Finally, we added more convolution layers in Encoder and transposed convolutional layers in Decoder and this worked effectively.
- In the training process, we have tried just using only one storm dataset and the combination of some of the storm datasets. We chose the best dateset in the end.

### Task 2 - Wind Speed
In Task 2, Team Nicholas was asked to train a network to predict wind speeds based on the training data provided. Each storm has time samples ordered chronologically. Each time sample has an associated jpg image, and associated feature file (.json) which contains the time-stamp, and a label file (also .json) with wind speed.

The results of Task 2 are saved under "Data" -> "results" folder.

#### Task 2 Models
Team Nicholas developed 4 models for Task 2:
1. **CNN**
  Applied well on training set. Works well on seen data but not so well on unseen data. It was not effective for sudden changes in wind speed. This model may have also been prone to overfitting.
  We discovered the importance of including the time sequence in further model attempts.

2. **RESNET- LSTM**
  This model was found to be too complex with RESNET architecture and there was not an effective output.

3. **CNN-LSTM**
  In this model, the features (of image and time) are combined before being fed into an LSTM and next fed through a linear layer. We decided to investigate a further CNN-LSTM model.

4. **CNN-LSTM (upgraded architecture)**
  In this model upgraded CNN-LSTM model, in one path the image features are fed into a CNN, in another path the time features are fed into an LSTM. The output of the LSTM and the CNN are combined then fed through a linear layer.

5. **Model on Surprise Storm**

  If we use the predicted first and second images as input to predict the third image, the clarity of the third image will be poor. Therefore, we train two models: the first model uses images from i to i+sequence-1 to predict the i+sequence image, and the second model uses images from i to i+sequence-1 to predict the i+sequence+1 image. This way, we can use model1 to predict the first image and model2 to predict the second and third images. This approach results in better clarity for the generated third image.

### User instructions
Task 1 uses Google Colab due to the required computing power. Task 2 can be run using a python workbook.

#### Task 1
If you would like to run Task 1, it would likely be suitable to use Google Colab given the computing power that is required. If downloading the Task 1 file (which can be found in folder "ACDS" -> "image"), the below code should be run at the top of your colab notebook"

``` from google.colab import drive
drive.mount('/content/drive') 
```

#### Task 2
Task 2 can be run using a python workbook, which can be found in folder "ACDS" -> "Speed"

#### Libraries required
These libraries would need to be installed if you do not have them already.
torch
os
PIL
tdqm
torch.nn
torch.nn.functional
torchvision
torch.utils.data
torch.utils.data
torch.nn
torch.nn.functional
matplotlib.pyplot
numpy
livelossplot
random
sklearn.metrics 
pandas
json

#### Original links
Original link to the training dataset
https://drive.google.com/drive/folders/1tFqoQl-sdK6qTY1vNWIoAdudEsG6Lirj?usp=drive_link

Original link to the surprise storm tst
https://drive.google.com/drive/folders/1HYHH7kBirVto9640I67Qh53Od_98Zo5z?usp=sharing

Original link to the briefing slides
https://imperiallondon.sharepoint.com/:p:/r/sites/TrialTeam-EA/Shared%20Documents/General/TheDayAfterTomorrow-presentation%202.pptx?d=wdf1d9e0210264eab88858e2353a36242&csf=1&web=1&e=XoU1Am