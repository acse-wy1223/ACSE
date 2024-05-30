import matplotlib.pyplot as plt
import torchvision.transforms as transforms
import torch
from ConvLSTMDataset import StormDataset
import torch.nn.functional as F


def save_tensor_as_jpeg(tensor, filename):
    """Save tensor as a JPEG image"""
    to_pil = transforms.ToPILImage()
    pil_img = to_pil(tensor.squeeze().byte())
    pil_img.save(filename)


def shift_images(images):
    """Shift image sequence forward by one"""
    for i in range(images.shape[1] - 1):
        images[0, i, :, :, :] = images[0, i + 1, :, :, :]
    images[0, -1, 0, :, :] = images[0, -2, 0, :, :]
    return images


def generate_future(future_step: int, dataset: StormDataset, model, gap=1, seq=5): # noqa
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    cnt = len(dataset)
    full_inputs, _ = dataset[cnt - 1]
    last_idx = cnt - gap
    inputs, _ = dataset[last_idx]

    images = inputs["image"].detach().unsqueeze(0).to(device)
    full_images = full_inputs["image"].detach().unsqueeze(0).to(device)

    pre = full_images[0, -1, 0, :, :]
    model.eval()
    if gap > 1:
        fig, axes = plt.subplots(1, gap, figsize=(12, 6))
    with torch.no_grad():
        images = images.to(device)
        # Initial update of the image sequence
        images = shift_images(images)
        for i in range(future_step):
            pic = model(images)
            # Convert to PIL Image
            pic = (pic + 1) / 2 * 255
            # Save as JPEG
            filename = "tst_" + str(cnt + i + seq + (0 if gap == 1 else gap)) + ".jpg" # noqa
            save_tensor_as_jpeg(pic, filename)

            pic = pic.squeeze().cpu().numpy()
            if gap > 1:
                axes[i].imshow(pic, "gray")
                images[0, -1, 0, :, :] = pre
                pre = torch.tensor(pic)
            else:
                plt.imshow(pic, "gray")
                images[0, -1, 0, :, :] = torch.tensor(pic)

            images = shift_images(images)


def gaussian_window(size, sigma):
    coords = torch.arange(size, dtype=torch.float32) - size // 2
    grid = torch.exp(-(coords**2) / (2 * sigma**2))
    grid /= grid.sum()
    return grid.view(1, -1) * grid.view(-1, 1)


def ssim(img1, img2, window_size=11, window_sigma=1.5, size_average=True):
    channel = img1.size(1)
    window = (
        gaussian_window(window_size, window_sigma)
        .to(img1.device)
        .repeat(channel, 1, 1, 1)
    )

    mu1 = F.conv2d(img1, window, padding=window_size // 2, groups=channel)
    mu2 = F.conv2d(img2, window, padding=window_size // 2, groups=channel)

    mu1_sq = mu1.pow(2)
    mu2_sq = mu2.pow(2)
    mu1_mu2 = mu1 * mu2

    sigma1_sq = (
        F.conv2d(img1 * img1, window, padding=window_size // 2, groups=channel) - mu1_sq # noqa
    )
    sigma2_sq = (
        F.conv2d(img2 * img2, window, padding=window_size // 2, groups=channel) - mu2_sq # noqa
    )
    sigma12 = (
        F.conv2d(img1 * img2, window, padding=window_size // 2, groups=channel)
        - mu1_mu2
    )

    C1 = 0.01**2
    C2 = 0.03**2

    ssim_map = ((2 * mu1_mu2 + C1) * (2 * sigma12 + C2)) / (
        (mu1_sq + mu2_sq + C1) * (sigma1_sq + sigma2_sq + C2)
    )

    if size_average:
        return ssim_map.mean()
    else:
        return ssim_map.mean(1).mean(1).mean(1)


class SSIMLoss(torch.nn.Module):
    def __init__(self, window_size=11, size_average=True):
        super(SSIMLoss, self).__init__()
        self.window_size = window_size
        self.size_average = size_average

    def forward(self, img1, img2):
        return 1 - ssim(img1, img2, self.window_size, size_average=self.size_average) # noqa
