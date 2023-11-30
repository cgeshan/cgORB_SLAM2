#include <algorithm>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <System.h>

#include "DataStream.h"
#include "PNGHandler.h"

int main(int argc, char **argv)
{
    DataStream stream;
    stream.SetFileName("Custom/data_stream");
    stream.SetPermission(0666);

<<<<<<< HEAD
    PNGHandler imgRGB, imgDepth;
    imgRGB.SetResolution(224, 224, 3);
    imgDepth.SetResolution(224, 224, 3);

    ORB_SLAM2::System SLAM(argv[1], argtv[2], ORB_SLAM2::System::RGBD, true);
=======
    PNGHandler imgDepth;
    imgDepth.SetResolution(640, 480, 1);
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff

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

    // Buffer reading
    std::vector<char> buffer;
<<<<<<< HEAD
    size_t desiredBytes = image.GetSize();
    size_t totalBytes = desiredBytes * 2;
    char tempBuffer[desiredBytes];
=======
    size_t totalBytes = imgDepth.GetSize() * 5;
    char tempBuffer[totalBytes];

>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
    size_t bytesRead;

    // Timing
    std::vector<float> vElapsedTime;

    while (!stream.terminate)
    {
<<<<<<< HEAD
=======

>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
        bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        totalBytes -= bytesRead;

        if (0 == totalBytes)
        {
            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

<<<<<<< HEAD
            std::cout << "\n## First pair of images received..." << std::endl;

            // Split the bytes into respective images
            std::cout << "## Splitting buffer into RGB and Depth images..." << std::endl;
            std::vector<char> bufferRGB, bufferDepth;
            bufferRGB.insert(bufferRGB.begin(), tempBuffer, tempBuffer + desiredBytes);
            bufferDepth.insert(bufferDepth.begin(), tempBuffer + desiredBytes + 1, tempBuffer + bytesRead);

            // Assign buffer to image matrices
            imgRGB.SetImageMatrix(bufferRGB);
            imgDepth.SetImageMatrix(bufferDepth);

// Run ORB-SLAM2 Track RGB-D
#ifdef COMPILEDWITHC11
            std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
            std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

            // Pass the image to the SLAM system
            SLAM.TrackRGBD(imgRGB, imgDepth, static_cast<double> timestamp);

#ifdef COMPILEDWITHC11
            std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
            std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

            // Clear all buffers
            buffer.clear();
            bufferRGB.clear();
            bufferDepth.clear();

            totalBytes = desiredBytes * 2;

            double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
            vElapsedTime.push_back(elapsedTime);
=======
            std::vector<char> depthBuffer(buffer.begin(), buffer.end());
            imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
            imgDepth.SetData(depthBuffer);
            imgDepth.Save();

            depthBuffer.clear();
            buffer.clear();
            totalBytes = imgDepth.GetSize();
            std::cout << "Exiting saving loop..." << std::endl;
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
        }

        std::string bytesStr(tempBuffer, bytesRead);

        if ("terminate" == bytesStr)
        {
            stream.terminate = true;
            std::cout << "## Terminate command received, closing pipe and exiting data stream loop..." << std::endl;
            break;
        }
    }
    SLAM.Shutdown();
    stream.Close();

    float totalElapsedTime = std::accumulate(vElapsedTime.begin(), vElapsedTime.end(), 0);
    std::cout << "\n## Median Tracking Time: " << vElapsedTime[vElapsedTime.size() / 2] << std::endl;
    std::cout << "## Mean Tracking Time: " << totalElapsedTime / vElapsedTime.size() << std::endl;

    std::string filepath = "./output/RGB_D/";
    std::string path = argv[3];
    size_t lastSlashPos = path.find_last_of('/');
    std::string extractedPath = path.substr(12, lastSlashPos - 12);

    SLAM.SaveTrajectoryTUM(filepath + extractedPath);
    SLAM.SaveKeyFrameTrajectoryTUM(filepath + extractedPath);

    return 0;
}

/**
 * Get the camera resolution from the yaml settings file.
 * cv::FileStorage fSettings(argv[2], cv::FileStorage::READ);
 * image.SetResolution(fSettings["Camera.width"], fSettings["Camera.height"], fSettings["Camera.channels"]);
 */

<<<<<<< HEAD
/**
 * ORB-SLAM2 needs both an rgb and a depth image.
 * SLAM.TrackRGBD(const cv::Mat &rgb, const cv::Mat &depth, const double &timestamp. )
 * Time stamp can be arbitrary I believe, thus we can use chrono or simply iterative idx
 * I believe this is implemented above.
 */

/**
 * Below is original working code for reading images, sent to pipe one at a time.
 * Starting with Depth then RGB
 * Lines 65-79
 *
 */
// PNGHandler image;
// image.SetResolution(224, 224, 3);
// image.type = DEPTH_IMAGE;
// bytesRead = read(stream.fileDescriptor, tempBuffer, desiredBytes);

// desiredBytes -= bytesRead;

// if (0 == desiredBytes)

// if (DEPTH_IMAGE == image.type)
// {
//     image.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
//     image.type = RGB_IMAGE;
// }
// else if (RGB_IMAGE == image.type)
// {
//     image.SetFilename("Custom/rgb/" + std::to_string(timestamp) + ".png");
//     image.type = DEPTH_IMAGE;
// }

// image.SetImageMatrix(buffer);
// image.Save();
// buffer.clear();
// desiredBytes = image.GetSize();
=======
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
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
