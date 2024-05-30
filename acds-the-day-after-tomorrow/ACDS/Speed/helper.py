import matplotlib.pyplot as plt
import pandas as pd
import re
import json
import random # noqa
from pathlib import Path
from PIL import Image
import numpy as np
from collections import OrderedDict # noqa
import torch
from torch import nn, optim # noqa
import os
import re # noqa
import json # noqa
import pandas as pd # noqa
from PIL import Image # noqa
import numpy as np # noqa
from concurrent.futures import ProcessPoolExecutor
from livelossplot import PlotLosses
from torchvision import transforms
import cv2


def process_storm_directory(storm_folder_path):
    data_list = []
    for entry in os.scandir(storm_folder_path):
        if entry.is_file() and entry.name.endswith((".jpg", ".jpeg")):
            split_names = re.split(r"[_.]", entry.name)
            split_names = [part for part in split_names if part]
            name = split_names[0]
            num = split_names[1]
            label_file = os.path.join(
                storm_folder_path, name + "_" + num + "_label.json"
            )
            with open(label_file, "r") as json_file:
                json_data = json.load(json_file)
                wind_speed = json_data["wind_speed"]
            features_file = os.path.join(
                storm_folder_path, name + "_" + num + "_features.json"
            )
            with open(features_file, "r") as json_file:
                json_data = json.load(json_file)
                relative_time = json_data["relative_time"]
                ocean = json_data["ocean"]
                storm_name = json_data["storm_id"]
            data_list.append(
                {
                    "Storm_Name": storm_name,
                    "Id": int(num),
                    "Wind_Speed": int(wind_speed),
                    "Relative_time": int(relative_time),
                    "Ocean": int(ocean),
                    "Image_Path": os.path.join(storm_folder_path, entry.name),
                }
            )
    return data_list


def get_raw_data(prefix="./data/Selected_Storms_curated_to_zip/"):
    data_list = []
    with ProcessPoolExecutor() as executor:
        futures = [
            executor.submit(process_storm_directory, os.path.join(prefix, storm_dir)) # noqa
            for storm_dir in os.listdir(prefix)
            if storm_dir != ".DS_Store"
            and os.path.isdir(os.path.join(prefix, storm_dir))
        ]

        for future in futures:
            data_list.extend(future.result())
    return pd.DataFrame(data_list)


def save_image_pil(image, path):
    """
    Saves an image given as a numpy array or torch tensor to a file.

    Parameters:
    image: Image data. Can be a numpy array or torch tensor. Expected shape [H, W, C] for numpy and [C, H, W] for tensor. # noqa
    path (str): The path where to save the image.
    """
    if isinstance(image, torch.Tensor):
        # Convert tensor to numpy array
        image = (
            image.permute(1, 2, 0).cpu().numpy()
        )  # Change from C x H x W to H x W x C
        image = (image * 255).clip(0, 255).astype(np.uint8)
    elif isinstance(image, np.ndarray):
        image = (image * 255).clip(0, 255).astype(np.uint8)
    else:
        raise TypeError("image must be a torch.Tensor or numpy array")

    # Save the image
    im = Image.fromarray(image)
    im.save(path)


def save_model(
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer,
    checkpoint_dir: str,
    model_name: str,
    model_version: str,
    epoch=None,
    log=None,
    info=False,
):
    # Create target directory
    checkpoint_dir_path = Path(checkpoint_dir)
    checkpoint_dir_path.mkdir(parents=True, exist_ok=True)

    # Create model path
    if epoch is not None:
        model_name = f"{model_name}_{model_version}_{epoch}.pth"
    else:
        model_name = f"{model_name}_{model_version}.pth"

    model_save_path = checkpoint_dir_path / model_name
    checkpoint = {
        "model_state_dict": model.state_dict(),
        "optimizer_state_dict": optimizer.state_dict(),
        "epoch": epoch,
    }
    if log is not None:
        checkpoint["log"] = log

    torch.save(obj=checkpoint, f=model_save_path)
    # Save the model state_dict()
    if info:
        print(f"[INFO] Saving model to: {model_save_path}")


def load_model(
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer,
    checkpoint_dir: str,
    checkpoint: str,
    info=False,
):
    checkpoint = f"{checkpoint}.pth"
    load_path = Path(checkpoint_dir) / checkpoint
    print(load_path)
    checkpoint = torch.load(load_path)
    model.load_state_dict(checkpoint["model_state_dict"])
    optimizer.load_state_dict(checkpoint["optimizer_state_dict"])
    epoch = checkpoint["epoch"]
    log = None
    if "log" in checkpoint:
        log = checkpoint["log"]
    if info:
        print(f"[INFO] Loading model from: {load_path} at epoch: {epoch}")
    return model, optimizer, epoch, log


def train_and_eval(
    model,
    train_loader,
    test_loader,
    criterion,
    optimizer,
    n_epochs,
    device="cpu",
    logs=None,
    checkpoint_dir="./models/",
    model_name="cnn_lstm",
    model_version="v1",
    checkpoint=None,
    save_inv=5,
    live_plot=False,
    train_info=False,
):

    checkpoint_dir = Path(checkpoint_dir) / model_name
    start_epoch = 0
    model.to(device)
    model.train()
    if live_plot:
        if logs is None:
            logs = {"train_losses": [], "train_targets": [], "train_predicts": []} # noqa
        groups = {"loss": ["Train_Loss"], "speed": ["Train_Target", "Train_Predict"]} # noqa

        liveloss = PlotLosses(groups=groups)
        # set liveloss x-axis name as 'iteration'
        liveloss.update({})
    # load checkpoint
    if checkpoint is not None:
        model, optimizer, start_epoch, log_old = load_model(
            model, optimizer, checkpoint_dir, checkpoint, info=True
        )
        # liveloss loads the previous epoch
        if live_plot:
            for _ in range(start_epoch + 1):
                liveloss.update({})

    for epoch in tqdm( # noqa
        range(start_epoch, start_epoch + n_epochs),
        initial=start_epoch,
        total=start_epoch + n_epochs,
    ):
        current_epoch = epoch + 1
        log = {}
        train_loss = 0.0

        for i, sample in enumerate(train_loader):
            if model.num_features > 0:
                images, additional_features, wind_speeds = (
                    sample[0].to(device),
                    sample[1].to(device),
                    sample[2].to(device),
                )
                outputs = model(images, additional_features)
            else:
                images, wind_speeds = sample[0].to(device), sample[1].to(device) # noqa
                outputs = model(images)

            # reset the parameter gradients
            optimizer.zero_grad()
            # print(outputs.shape, wind_speeds.shape)
            loss = criterion(outputs, wind_speeds.to(device))
            loss.backward()

            optimizer.step()
            train_loss += loss.item()
            # global_step += 1
            # update live plot
            if live_plot:
                log["Train_Loss"] = loss.item()
                log["Train_Target"] = wind_speeds.item()
                log["Train_Predict"] = outputs.item()
                liveloss.update(log)
                liveloss.send()

            if (i + 1) % 50 == 0:
                print(
                    f"Epoch: {epoch}, Loss: {loss.item():.4f}, Predicted: {outputs.item()}, Actual: {wind_speeds.item()}" # noqa
                )
        # avg train loss
        print(
            f"Epoch: {epoch}, Average Loss: {train_loss / len(train_loader):.4f}, Total Loss: {train_loss:.4f}" # noqa
        )

        # save model every save_inv epochs
        if current_epoch % save_inv == 0:
            save_model(
                model,
                optimizer,
                checkpoint_dir,
                model_name,
                model_version,
                epoch=current_epoch,
                log=log,
            )
        model.train()


def score_matrix(model, data_loader, criterion, device="cpu"):
    model.to(device)
    model.eval()
    test_loss = 0
    targets = []
    predicts = []

    with torch.no_grad():
        for sample in data_loader:
            if model.num_features > 0:
                images, additional_features, label = (
                    sample[0].to(device),
                    sample[1].to(device),
                    sample[2].to(device),
                )
                predict = model(images, additional_features)
            else:
                images, label = sample[0].to(device), sample[1].to(device)
                predict = model(images)
            # print(predict, label)
            loss = criterion(predict, label)
            test_loss += loss.item()
            targets.append(label.item())
            predicts.append(predict.item())

    test_loss /= len(data_loader)

    return test_loss, targets, predicts


def extract_id_from_filename(filename):
    # Assuming the ID is the part between the last underscore and the dot
    if filename[-3:] == "jpg":
        match = re.search(r"_(\d+)\.", filename)
    else:
        match = re.search(r"_(\d+)_", filename)
    return int(match.group(1)) if match else None


def read_images_recursive(folder_path):
    image_data = []
    for root, dirs, files in os.walk(folder_path):
        folder_name = os.path.basename(root)
        for file in files:
            if file.endswith((".jpg", ".jpeg")):
                image_path = os.path.join(root, file)
                image_id = extract_id_from_filename(file)
                data = {
                    "Id": image_id,
                    "Storm_Name": folder_name,
                    "Image_Path": image_path,
                }
                image_data.append(data)
    return image_data


def read_feature_jsons_recursive(folder_path):
    feature_data_list = []
    for root, dirs, files in os.walk(folder_path):
        folder_name = os.path.basename(root)
        for file in files:
            if file.endswith("_features.json"):
                image_id = extract_id_from_filename(file)
                with open(os.path.join(root, file), "r") as json_file:
                    json_data = json.load(json_file)
                    data = {
                        "Id": image_id,
                        "Storm_Name": folder_name,
                        "Relative_time": int(json_data["relative_time"]),
                        "Ocean": int(json_data["ocean"]),
                    }
                    feature_data_list.append(data)
    return feature_data_list


def read_label_jsons_recursive(folder_path):
    label_data_list = []
    for root, dirs, files in os.walk(folder_path):
        folder_name = os.path.basename(root)
        for file in files:
            if file.endswith("_label.json"):
                image_id = extract_id_from_filename(file)
                with open(os.path.join(root, file), "r") as json_file:
                    json_data = json.load(json_file)
                    data = {
                        "Id": image_id,
                        "Storm_Name": folder_name,
                        "Wind_Speed": json_data["wind_speed"],
                    }
                    label_data_list.append(data)
    return label_data_list


def plot_transformed_images(image_path, t):
    # open grey scale image
    with Image.open(image_path).convert("L") as f:
        fig, ax = plt.subplots(1, 2)
        ax[0].imshow(f, cmap="gray")
        ax[0].set_title(f"Original \nSize: {f.size}")
        ax[0].axis("off")

        transformed_image = t(f)
        transformed_image = np.array(transformed_image)[0]
        ax[1].imshow(transformed_image, cmap="gray")
        ax[1].set_title(f"Transformed \nSize: {transformed_image.shape}")
        ax[1].axis("off")


def transformer(image_size=366, repair=False):
    if repair:
        transform = transforms.Compose(
            [
                transforms.Grayscale(),
                transforms.Resize((image_size, image_size)),
                RepairImageTransform(),
                transforms.ToTensor(),
                transforms.Normalize(mean=[0.485], std=[0.229]),
                # transforms.Grayscale(),
            ]
        )
    else:
        transform = transforms.Compose(
            [
                transforms.Grayscale(),
                transforms.Resize((image_size, image_size)),
                # RepairImageTransform(),
                transforms.ToTensor(),
                transforms.Normalize(mean=[0.485], std=[0.229]),
                # transforms.Grayscale(),
            ]
        )

    return transform


class RepairImageTransform:
    def __init__(self, threshold=240, radius=9, kernel_size=3):
        self.threshold = threshold
        self.radius = radius
        self.kernel_size = kernel_size

    def __call__(self, image):
        # Convert PIL image to numpy array for processing
        image_array = np.array(image)

        # Apply threshold to create a mask for inpainting
        _, mask = cv2.threshold(image_array, self.threshold, 255, cv2.THRESH_BINARY) # noqa

        # Use inpainting to repair the image
        repaired_image = cv2.inpaint(image_array, mask, self.radius, cv2.INPAINT_TELEA) # noqa

        # Apply median blur to reduce noise
        denoised_image = cv2.medianBlur(repaired_image, self.kernel_size)

        # Convert the processed numpy array back to a PIL Image
        # Ensure it stays as a grayscale image
        result_image = Image.fromarray(denoised_image)

        return result_image
