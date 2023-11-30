from PIL import Image

import numpy as np
import h5py
import argparse
import os


def h5_loader(path):
    h5f = h5py.File(path, "r")
    rgb = np.array(h5f["rgb"])
    rgb = np.transpose(rgb, (1, 2, 0))
    depth = np.array(h5f["depth"])
    return rgb, depth


def array_to_png(arr, path, img_type):
    img = Image.fromarray(arr.astype("uint8"))
    fn = os.path.splitext(os.path.basename(path))[0]
    save_path = os.path.join(fn, f"{img_type}.png")

    return img, save_path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--data",
        metavar="DATA",
        default="",
        required=True,
        help="dataset of images in .h5 format.",
    )

    args = parser.parse_args()

    for file_path in os.listdir(args.data):
        rgb_img, depth_img = h5_loader(file_path)
        img, path = array_to_png(rgb_img, file_path, "rgb")
        img.save(path)

        img, path = array_to_png(depth_img, file_path, "depth")
        img.save(path)


if __name__ == "__main__":
    main()
