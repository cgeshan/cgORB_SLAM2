from PIL import Image
import numpy as np
import os
import time


# Create a named pipe (FIFO) if it doesn't exist
fifo_path = "Custom/data_stream"
if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

with open(fifo_path, "wb") as fifo:
    for _ in range(30):
        img = Image.open(
            "/home/cgeshan/Desktop/CMU/F23/cgORB_SLAM2/Custom/ground_truth.png"
        )

        rgb_arr = np.array(img)
        fifo.write(rgb_arr.tobytes())
        print("GT Sent")
        # time.sleep(2)

        img2 = Image.open("/home/cgeshan/Desktop/CMU/F23/cgORB_SLAM2/Custom/pred.png")

        rgb_arr2 = np.array(img2)
        fifo.write(rgb_arr2.tobytes())
        print("Pred Sent")

    terminate_signal = "terminate"
    fifo.write(terminate_signal.encode("utf-8"))
    print("terminate sent")

    fifo.close()
