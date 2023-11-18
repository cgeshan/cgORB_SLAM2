#include <algorithm>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

// #include <System.h>

#include "DataStream.h"
#include "PNGHandler.h"

int main(int argc, char **argv)
{
    DataStream stream;
    stream.SetFileName("Custom/data_stream");
    stream.SetPermission(0666);

    PNGHandler image;
    image.SetResolution(224, 224, 3);
    image.type = DEPTH_IMAGE;

    if (!stream.Init())
    {
        std::cout << "*** ERROR *** Terminating program, pipe initialization failed." << std::endl;
        return 1;
    }

    stream.OpenReadOnly();

    if (stream.fileDescriptor == -1)
    {
        std::cerr << "Failed to open the named pipe." << std::endl;
        return 1;
    }

    std::vector<char> buffer;
    size_t desiredBytes = image.GetSize();
    char tempBuffer[desiredBytes];

    size_t bytesRead;

    while (!stream.terminate)
    {
        bytesRead = read(stream.fileDescriptor, tempBuffer, desiredBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        desiredBytes -= bytesRead;

        if (0 == desiredBytes)
        {
            // std::cout << "Image size from buffer: " << buffer.size() << ", Desired image resoltion: " << image.GetSize() << std::endl;
            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
            if (DEPTH_IMAGE == image.type)
            {
                image.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
                image.type = RGB_IMAGE;
            }
            else if (RGB_IMAGE == image.type)
            {
                image.SetFilename("Custom/rgb/" + std::to_string(timestamp) + ".png");
                image.type = DEPTH_IMAGE;
            }

            image.SetData(buffer);
            image.Save();
            buffer.clear();
            desiredBytes = image.GetSize();
        }

        std::string bytesStr(tempBuffer, bytesRead);

        if ("terminate" == bytesStr)
        {
            stream.terminate = true;
            std::cout << "## Terminate command received, closing pipe and exiting data stream loop..." << std::endl;
            break;
        }
    }

    stream.Close();

    return 0;
}

/**
 * Get the camera resolution from the yaml settings file.
 * cv::FileStorage fSettings(argv[2], cv::FileStorage::READ);
 * image.SetResolution(fSettings["Camera.width"], fSettings["Camera.height"], fSettings["Camera.channels"]);
 */