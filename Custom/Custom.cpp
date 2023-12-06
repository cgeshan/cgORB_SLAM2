#include <algorithm>
#include <chrono>
#include <tiffio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <System.h>

#include "DataStream.h"
#include "PNGHandler.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << endl
             << "* Improper Usage * ./Custom path_to_vocabulary path_to_camera_settings" << endl;
        return 1;
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1], argv[2], ORB_SLAM2::System::RGBD, true);

    std::cout << "\n\n## SLAM system initialized... " << std::endl;

    DataStream stream;
    stream.SetFileName("Custom/data_stream");
    stream.SetPermission(0666);

    const std::string strSettingsFile = argv[2];
    cv::FileStorage fsSettings(strSettingsFile, cv::FileStorage::READ);
    if (!fsSettings.isOpened())
    {
        cerr << "Failed to open settings file at: " << strSettingsFile << endl;
        exit(-1);
    }

    PNGHandler imgRGB, imgDepth;
    // Get the camera resolution from the yaml settings file.cv::FileStorage fSettings(argv[2], cv::FileStorage::READ);
    imgRGB.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], fsSettings["Camera.channels"], CV_8UC3);
    imgDepth.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], 1, CV_8UC1);

    if (!stream.Init())
    {
        std::cout << "*** ERROR *** Terminating program, pipe initialization failed." << std::endl;
        return 1;
    }

    std::cout << "## Plead run cgFastDepth/online.py for online SLAM using FastDepth estimation..." << std::endl;
    stream.OpenReadOnly();

    if (stream.fileDescriptor == -1)
    {
        std::cerr << "Failed to open the named pipe." << std::endl;
        return 1;
    }

    // Buffer reading
    std::vector<char> buffer;
    size_t totalBytes = imgRGB.GetSize() + imgDepth.GetSize();
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

            std::vector<char> rgbBuffer(buffer.begin(), buffer.begin() + imgRGB.GetSize());
            imgRGB.SetImageMatrixFromVector(rgbBuffer);

            std::vector<char> depthBuffer(buffer.begin() + imgRGB.GetSize(), buffer.end());
            imgDepth.SetImageMatrixFromVector(depthBuffer);

            // // If you would like to save the files
            // imgRGB.SetFilename("Custom/rgb/" + std::to_string(timestamp) + ".png");
            // imgRGB.Save();
            // imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
            // imgDepth.Save();

            // Pass the image to the SLAM system
            SLAM.TrackRGBD(imgRGB.GetImageMatrix(), imgDepth.GetImageMatrix(), timestamp);

            rgbBuffer.clear();
            depthBuffer.clear();
            buffer.clear();
            totalBytes = imgRGB.GetSize() + imgDepth.GetSize();
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

    return 0;
}