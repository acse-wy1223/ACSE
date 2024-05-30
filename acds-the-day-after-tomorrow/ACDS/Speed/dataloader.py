import matplotlib.pyplot as plt # noqa
import json # noqa 
import random # noqa
from pathlib import Path # noqa
from PIL import Image # noqa
import numpy as np # noqa
from collections import OrderedDict # noqa
from torch import nn, optim # noqa
import re # noqa
import pandas as pd
from concurrent.futures import ProcessPoolExecutor # noqa
import torch
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms
import os # noqa
from tqdm.auto import tqdm
import cv2 # noqa


class StormDataset(Dataset):
    def __init__(
        self,
        root_dir,
        raw_data_dir,
        storm_name,
        transform=None,
        split_ratio=0.9,
        sequence_length=5,
        additional_features=None,
        label_type="wind_speed",
        type="train",
    ):
        """
        root_dir: 存放所有风暴数据的文件夹路径
        transform: 对图像进行预处理的转换
        """
        self.root_dir = root_dir
        self.raw_data_dir = raw_data_dir
        self.storm_name = storm_name
        self.transform = transform
        if transform is None:
            self.transform = transforms.Compose(
                [
                    transforms.Grayscale(),
                    transforms.ToTensor(),
                ]
            )
        self.type = type
        self.split_ratio = split_ratio
        self.sequence_length = sequence_length
        self.additional_features = additional_features
        if additional_features is not None:
            self.num_features = len(additional_features)
        else:
            self.num_features = 0
        self.label_type = label_type
        self.samples, self.images_samples = self._get_samples()
        self.sample = None
        self.image_sample = None

    def _get_samples(self):

        raw_data = pd.read_csv(self.raw_data_dir)
        raw_data_sorted = raw_data.sort_values(by=["Storm_Name", "Relative_time"]) # noqa
        samples = raw_data_sorted[
            raw_data_sorted["Storm_Name"] == self.storm_name
        ].copy()
        split_index = int(len(samples) * self.split_ratio)
        if self.type == "train":
            samples = samples[:split_index]
        else:
            samples = samples[split_index:]

        images_list = samples["Image_Path"].tolist()
        # read image data
        images_samples = []

        for image_path in images_list:
            image = Image.open(image_path)
            if self.transform:
                image = self.transform(image)
            images_samples.append(image)
        return samples, images_samples

    def __getitem__(self, idx):

        images, additional_features_sequence = [], []
        label = None

        # 起始点为idx, 结束点为idx + sequence_length
        for i in range(idx, idx + self.sequence_length):

            self.sample = self.samples.iloc[i].tolist()
            self.image_sample = self.images_samples[i]

            images.append(self.image_sample)

            if self.additional_features is not None:
                additional_features_sequence.append(self.get_additional_features()) # noqa

        # 将列表转换为张量
        images = torch.stack(images)

        if self.additional_features is not None:
            additional_features_sequence = torch.stack(additional_features_sequence) # noqa

        # get label
        if self.label_type == "wind_speed":
            label = torch.tensor(float(self.sample[2]), dtype=torch.float)
        elif self.label_type == "image":
            label = self.image_sample.unsqueeze(0)

        # print(label)
        if self.additional_features is not None:
            return images[:-1], additional_features_sequence[:-1], label
        else:
            return images[:-1], label

    def __len__(self):
        # 因为我们需要连续的序列，所以总长度需要减去序列长度
        return len(self.samples) - self.sequence_length + 1

    def get_additional_features(self):
        storm_name, id, wind_speed, relative_time, ocean, image_path = self.sample # noqa
        additional_features_list = []

        for feature in self.additional_features:
            if feature == "wind_speed":
                additional_features_list.append(
                    torch.tensor(float(wind_speed), dtype=torch.float)
                )
                wind_speed = torch.tensor(float(wind_speed), dtype=torch.float)
            elif feature == "relative_time":
                additional_features_list.append(
                    torch.tensor(float(relative_time), dtype=torch.float)
                )
                relative_time = torch.tensor(float(relative_time), dtype=torch.float) # noqa
            elif feature == "ocean":
                additional_features_list.append(
                    torch.tensor(float(ocean), dtype=torch.float)
                )
            elif feature == "id":
                additional_features_list.append(
                    torch.tensor(float(id), dtype=torch.float)
                )

        return torch.stack(additional_features_list)


def train_test_split(
    root_dir,
    raw_data_dir,
    storm_name,
    transform,
    split_ratio,
    sequence_length,
    additional_features,
    label_type,
):
    train_dataset = StormDataset(
        root_dir=root_dir,
        raw_data_dir=raw_data_dir,
        storm_name=storm_name,
        transform=transform,
        split_ratio=split_ratio,
        sequence_length=sequence_length,
        additional_features=additional_features,
        label_type=label_type,
        type="train",
    )
    test_dataset = StormDataset(
        root_dir=root_dir,
        raw_data_dir=raw_data_dir,
        storm_name=storm_name,
        transform=transform,
        split_ratio=split_ratio,
        sequence_length=sequence_length,
        additional_features=additional_features,
        label_type=label_type,
        type="test",
    )
    return train_dataset, test_dataset


def make_datasets(
    root_dir,
    raw_data_dir,
    storm_names,
    transform=None,
    split_ratio=0.9,
    sequence_length=5,
    additional_features=None,
    label_type="wind_speed",
):

    datasets = {storm_name: [] for storm_name in storm_names}
    for storm_name in tqdm(storm_names):
        train_dataset, test_dataset = train_test_split(
            root_dir,
            raw_data_dir,
            storm_name,
            transform,
            split_ratio,
            sequence_length,
            additional_features,
            label_type,
        )
        datasets[storm_name].append(train_dataset)
        datasets[storm_name].append(test_dataset)
    return datasets


def make_dataloaders(datasets, storm_names, batch_size=1, shuffle=False):
    dataloaders = {storm_name: [] for storm_name in storm_names}
    for storm_name in tqdm(storm_names):
        train_loader = DataLoader(
            datasets[storm_name][0], batch_size=batch_size, shuffle=shuffle
        )
        test_loader = DataLoader(
            datasets[storm_name][1], batch_size=batch_size, shuffle=shuffle
        )
        dataloaders[storm_name].append(train_loader)
        dataloaders[storm_name].append(test_loader)
    return dataloaders
