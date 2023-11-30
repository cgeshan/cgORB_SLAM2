# import cv2
# import numpy as np


# def read_and_print_image(image_path):
#     # Read the image
#     img = cv2.imread(image_path)

#     if img is not None:
#         # Print the image array
#         print(f"Array for {image_path}:\n{img}\nShape: {img.shape}\n")
#     else:
#         print(f"Error: Unable to read image from {image_path}\n")


# # Paths to the images
# depth_image = "sequences/rgbd_dataset_freiburg1_desk/depth/1305031453.574074.png"
# fast_depth_image = (
#     "sequences/rgbd_dataset_freiburg1_desk/depth_preds/1305031453.627706.png"
# )


# # Read and print arrays for both images
# read_and_print_image(depth_image)
# read_and_print_image(fast_depth_image)


import cv2
import numpy as np
import matplotlib.pyplot as plt


def grayscale_to_depth(gray_image):
    # Assuming darker pixels are closer
    depth_image = 255 - gray_image
    return depth_image


# Load a grayscale image
gray_image = cv2.imread(
    "sequences/rgbd_dataset_freiburg1_desk/depth_preds/1305031453.627706.png",
    cv2.IMREAD_UNCHANGED,
)

# Convert grayscale to depth
depth_image = grayscale_to_depth(gray_image)

# Display the images
plt.subplot(1, 2, 1)
plt.imshow(gray_image)
plt.title("Grayscale Image")

plt.subplot(1, 2, 2)
plt.imshow(depth_image)  # 'jet' colormap for depth visualization
plt.title("Depth Image")

plt.show()
