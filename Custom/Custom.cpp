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

    const std::string strSettingsFile = argv[2];
    cv::FileStorage fsSettings(strSettingsFile.c_str(), cv::FileStorage::READ);
    if (!fsSettings.isOpened())
    {
        cerr << "Failed to open settings file at: " << strSettingsFile << endl;
        exit(-1);
    }

    PNGHandler imgDepth;
    // Get the camera resolution from the yaml settings file.cv::FileStorage fSettings(argv[2], cv::FileStorage::READ);
    imgDepth.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], fsSettings["Camera.channels"]);

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
    size_t totalBytes = imgDepth.GetSize() * 5;
    char tempBuffer[totalBytes];

    size_t bytesRead;

    // Timing
    std::vector<float> vElapsedTime;

    while (!stream.terminate)
    {
        bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        totalBytes -= bytesRead;

        bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        totalBytes -= bytesRead;
        if (0 == totalBytes)
        {
            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

            std::vector<char> depthBuffer(buffer.begin(), buffer.end());
            imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".png");
            imgDepth.SetImageMatrixFromVector(depthBuffer);
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
    // SLAM.Shutdown();
    stream.Close();

    // float totalElapsedTime = std::accumulate(vElapsedTime.begin(), vElapsedTime.end(), 0);
    // std::cout << "\n## Median Tracking Time: " << vElapsedTime[vElapsedTime.size() / 2] << std::endl;
    // std::cout << "## Mean Tracking Time: " << totalElapsedTime / vElapsedTime.size() << std::endl;

    // std::string filepath = "./output/RGB_D/";
    // std::string path = argv[3];
    // size_t lastSlashPos = path.find_last_of('/');
    // std::string extractedPath = path.substr(12, lastSlashPos - 12);

    // SLAM.SaveTrajectoryTUM(filepath + extractedPath);
    // SLAM.SaveKeyFrameTrajectoryTUM(filepath + extractedPath);

    return 0;
}