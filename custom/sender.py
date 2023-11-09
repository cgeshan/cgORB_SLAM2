import numpy as np
import random
from time import sleep
import os.path
import os
import time

pipe_name = "data_stream"

if os.path.exists(pipe_name):
    pipe_fd = os.open(pipe_name, os.O_WRONLY)
    print(f"File Descriptor: {pipe_fd}")

    for _ in range(4):
        feed = []
        for _ in range(4):
            feed.append(random.randint(0, 255))

        os.write(pipe_fd, bytes(" ".join(map(str, feed)), "utf-8"))
        print(f"Sent: {feed}")

        time.sleep(1)

    os.write(pipe_fd, bytes("".join(map(str, "terminate")), "utf-8"))
    os.close(pipe_fd)
