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

    PNGHandler imgDepth;
    imgDepth.SetResolution(640, 480, 1);

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
    size_t totalBytes = imgDepth.GetSize() * 5;
    char tempBuffer[totalBytes];

    size_t bytesRead;

    while (!stream.terminate)
    {

        bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        totalBytes -= bytesRead;

        if (0 == totalBytes)
        {
            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

            std::vector<char> depthBuffer(buffer.begin(), buffer.end());
            imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
            imgDepth.SetData(depthBuffer);
            imgDepth.Save();

            depthBuffer.clear();
            buffer.clear();
            totalBytes = imgDepth.GetSize();
            std::cout << "Exiting saving loop..." << std::endl;
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

// Working save RGB image from desk1 dataset

// #include <algorithm>
// #include <chrono>

// #include <opencv2/core/core.hpp>
// #include <opencv2/opencv.hpp>

// // #include <System.h>

// #include "DataStream.h"
// #include "PNGHandler.h"

// int main(int argc, char **argv)
// {
//     DataStream stream;
//     stream.SetFileName("Custom/data_stream");
//     stream.SetPermission(0666);

//     PNGHandler imgRGB, imgDepth;
//     imgRGB.SetResolution(640, 480, 3);
//     imgDepth.SetResolution(640, 480, 3);

//     if (!stream.Init())
//     {
//         std::cout << "*** ERROR *** Terminating program, pipe initialization failed." << std::endl;
//         return 1;
//     }

//     stream.OpenReadOnly();

//     if (stream.fileDescriptor == -1)
//     {
//         std::cerr << "Failed to open the named pipe." << std::endl;
//         return 1;
//     }

//     std::vector<char> buffer;
//     size_t totalBytes = imgDepth.GetSize() + imgRGB.GetSize();
//     char tempBuffer[totalBytes];

//     size_t bytesRead;

//     while (!stream.terminate)
//     {

//         bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
//         buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
//         totalBytes -= bytesRead;

//         if (0 == totalBytes)
//         {
//             std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
//             std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

//             std::vector<char> depthBuffer(buffer.begin(), buffer.begin() + imgDepth.GetSize());
//             imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
//             imgDepth.SetData(depthBuffer);
//             imgDepth.Save();

//             std::vector<char> rgbBuffer(buffer.begin() + imgDepth.GetSize(), buffer.end());
//             imgRGB.SetFilename("Custom/rgb/" + std::to_string(timestamp) + ".png");
//             imgRGB.SetData(rgbBuffer);
//             imgRGB.Save();

//             depthBuffer.clear();
//             rgbBuffer.clear();
//             buffer.clear();
//             totalBytes = imgDepth.GetSize() + imgRGB.GetSize();
//         }

//         std::string bytesStr(tempBuffer, bytesRead);

//         if ("terminate" == bytesStr)
//         {
//             stream.terminate = true;
//             std::cout << "## Terminate command received, closing pipe and exiting data stream loop..." << std::endl;
//             break;
//         }
//     }

//     stream.Close();

//     return 0;
// }

// Working save depth image from desk1 dataset