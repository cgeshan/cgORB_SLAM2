from PIL import Image
import numpy as np
import os
import time


# Create a named pipe (FIFO) if it doesn't exist
fifo_path = "Custom/data_stream"
if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

with open(fifo_path, "wb") as fifo:
    for _ in range(3):
        img2 = Image.open(
            "/home/cgeshan/Desktop/CMU/F23/cgORB_SLAM2/cgFastDepth/depth_preds/rgb.png"
        )

        rgb_arr2 = np.array(img2)
        fifo.write(rgb_arr2.tobytes())
        print("RGB Sent")
        print(rgb_arr2.shape)

        img3 = Image.open(
            "/home/cgeshan/Desktop/CMU/F23/cgORB_SLAM2/cgFastDepth/depth_preds/rgb_pred.png"
        )

        rgb_arr3 = np.array(img3)
        fifo.write(rgb_arr3.tobytes())
        print("depth Sent")

        # time.sleep(5)

    terminate_signal = "terminate"
    fifo.write(terminate_signal.encode("utf-8"))
    print("terminate sent")

    fifo.close()
