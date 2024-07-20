from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

# Load the image
img = Image.open("Images/gracehopper.png")
img = Image.open("tests/data/test_image_3channel.png")

# Convert the image to a numpy array
img_array = np.array(img)

# Print the shape of the array to verify the dimensions and channels
print("Shape of the array:", img_array.shape)

# Display the image to verify

# Extract the R channel values from the image array
r_channel = img_array[:, :, 0]
print(r_channel)

g_channel = img_array[:, :, 1]
print(g_channel)

b_channel = img_array[:, :, 2]
print(b_channel)

# # plot image.
plt.imshow(img_array[:, :, :])
plt.show()
