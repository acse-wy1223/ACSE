from torchvision import transforms
import cv2 # noqa
import numpy as np
from PIL import Image


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
        _, mask = cv2.threshold(image_array, self.threshold, 255, cv2.THRESH_BINARY)  # noqa

        # Use inpainting to repair the image
        repaired_image = cv2.inpaint(image_array, mask, self.radius, cv2.INPAINT_TELEA) # noqa

        # Apply median blur to reduce noise
        denoised_image = cv2.medianBlur(repaired_image, self.kernel_size)

        # Convert the processed numpy array back to a PIL Image
        # Ensure it stays as a grayscale image
        result_image = Image.fromarray(denoised_image)

        return result_image
