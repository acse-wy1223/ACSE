import os
import cv2
import json
import re
from torch.utils.data import Dataset
import pandas as pd
from PIL import Image
import torchvision.transforms as transforms
from sklearn.preprocessing import MinMaxScaler
import torch
import numpy as np


class StormDataset(Dataset):
    """
    A dataset class for storm image sequences with additional
    features for each image.

    Parameters:
    - root (str): Root directory for dataset.
    - storm (str): Specific storm to load data for.
    - sequence_length (int): Length of the sequence of images for each sample.
    - alldata (bool): Flag to indicate whether to use all data.
    Currently not used.
    - time_gap (int): Gap between sequences to predict next image.
    """

    def __init__(
        self, root: str, storm: str, sequence_length: int, alldata=False, time_gap=1 # noqa
    ) -> None:
        self.sequence_length = sequence_length
        self.prefix = root
        self.storm_name = storm
        self.time_gap = time_gap

        # Image preprocessing transformations
        self.transform = transforms.Compose(
            [
                transforms.Resize((360, 360)),
                transforms.Grayscale(num_output_channels=1),
                transforms.ToTensor(),
                transforms.Normalize(mean=[0.485], std=[0.229]),
            ]
        )

        # Load and preprocess data
        self.data, self.preprocessed_images = self._load_data()
        self.data.sort_values(by="Relative_Time", inplace=True)

        # Time scaler for normalization
        self.time_scaler = MinMaxScaler()
        self.data["Relative_Time"] = self.time_scaler.fit_transform(
            self.data[["Relative_Time"]]
        )
        self.data["Time_Diff"] = self.data["Relative_Time"].diff()
        self.data["Time_Diff"].fillna(0, inplace=True)

    def _load_data(self):
        """
        Loads and preprocesses the data from the specified directory.

        Returns:
        A tuple containing a DataFrame with the data and
        a dictionary of preprocessed images.
        """
        datalist = []
        preprocessed_images = {}

        storm_dir = os.path.join(self.prefix, self.storm_name)
        if os.path.isdir(storm_dir):
            image_files = [
                file
                for file in os.listdir(storm_dir)
                if file.endswith((".jpg", ".jpeg"))
            ]
            for im in image_files:
                split_names = re.split(r"[_.]", im)
                split_names = [part for part in split_names if part]
                name, num = split_names[0], split_names[1]

                image_path = os.path.join(storm_dir, im)
                image = Image.open(image_path)
                preprocessed_images[num] = self.transform(image)
                json_data = self._load_json(os.path.join(storm_dir, name + "_" + num)) # noqa
                json_data["Id"] = num
                datalist.append(json_data)

        return pd.DataFrame(datalist), preprocessed_images

    def _load_json(self, file_prefix):
        """
        Loads JSON data corresponding to an image.

        Parameters:
        - file_prefix (str): Prefix for the JSON file to load.

        Returns:
        A dictionary with loaded JSON data.
        """
        with open(file_prefix + "_features.json", "r") as json_file:
            features_data = json.load(json_file)

        return {
            "Storm_Name": features_data["storm_id"],
            "Relative_Time": int(features_data["relative_time"]),
            "Ocean": int(features_data["ocean"]),
            "Image_Path": file_prefix + ".jpg",
        }

    def __len__(self):
        """Returns the number of samples in the dataset."""
        return len(self.data) - self.sequence_length - self.time_gap + 1

    def __getitem__(self, idx):
        """
        Retrieves a sample from the dataset at the specified index.

        Parameters:
        - idx (int): Index of the sample to retrieve.

        Returns:
        A tuple of sample input data and the target next image.
        """
        if idx + self.sequence_length + self.time_gap - 1 >= len(self.data):
            idx = len(self.data) - self.sequence_length - self.time_gap + 1

        sequence_data = self.data.iloc[idx : idx + self.sequence_length] # noqa
        images = [self.preprocessed_images[str(id)] for id in sequence_data["Id"]] # noqa

        sample_input = {
            "image": torch.stack(images),
            "relative_time": torch.tensor(
                sequence_data["Relative_Time"].to_numpy(), dtype=torch.float32
            ),
            "time_diff": torch.tensor(
                sequence_data["Time_Diff"].to_numpy(), dtype=torch.float32
            ),
        }
        next_image_id = self.data.iloc[idx + self.sequence_length + self.time_gap - 1][ # noqa
            "Id"
        ]
        next_image = self.preprocessed_images[str(next_image_id)]

        return sample_input, next_image

    def __str__(self):
        """Returns a string representation of the
        class including the length and attributes,
        excluding 'data' and 'preprocessed_images'."""
        class_string = self.__class__.__name__ + "\n\tlen : %d" % self.__len__() # noqa
        for key, value in self.__dict__.items():
            if key not in ["data", "preprocessed_images"]:
                class_string += "\n\t" + str(key) + " : " + str(value)
        return class_string


class RepairImageTransform:
    def __init__(self, threshold=240, radius=9, kernel_size=3):
        self.threshold = threshold
        self.radius = radius
        self.kernel_size = kernel_size

    def __call__(self, image):
        # Convert image to array
        image = np.array(image)

        # Threshold to mark bright stripes
        _, thresh = cv2.threshold(image, self.threshold, 255, cv2.THRESH_BINARY) # noqa

        # Inpaint to repair image
        repaired_image = cv2.inpaint(image, thresh, self.radius, cv2.INPAINT_TELEA) # noqa

        # Median blur to reduce noise
        denoised_image = cv2.medianBlur(repaired_image, self.kernel_size)

        # Convert back to PIL Image and ensure RGB format
        denoised_image = Image.fromarray(denoised_image).convert("RGB")

        return denoised_image
