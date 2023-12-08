#include <algorithm>
#include <chrono>
#include <tiffio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <System.h>

#include "DataStream.h"
#include "PNGHandler.h"

void LoadTimeStamps(const string &strFile, vector<double> &timestamps)
{
    std::ifstream f;
    f.open(strFile.c_str());

    while (!f.eof())
    {
        std::string s;
        std::getline(f, s);
        if (!s.empty())
        {
            std::stringstream ss;
            ss << s;
            double t;
            ss >> t;

            // Adjust this based on format of timestamps (t) for TUM datasets
            timestamps.push_back(t);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << std::endl
                  << "* Improper Usage * ./Custom path_to_vocabulary path_to_camera_settings path_to_txt" << endl;
        return 1;
    }

    std::vector<double> timestamps;
    std::string strFile = std::string(argv[3]) + "/timestamps.txt";
    LoadTimeStamps(strFile, timestamps);
    int numImages = timestamps.size();

    // Create SLAM system.
    ORB_SLAM2::System SLAM(argv[1], argv[2], ORB_SLAM2::System::RGBD, true);
    std::cout << "\n## SLAM system initialized... " << std::endl;

    // Create named pipe for data stream
    DataStream stream;
    stream.SetFileName("Custom/data_stream");
    stream.SetPermission(0666);

    const std::string strSettingsFile = argv[2];
    cv::FileStorage fsSettings(strSettingsFile, cv::FileStorage::READ);
    if (!fsSettings.isOpened())
    {
        std::cerr << "Failed to open settings file at: " << strSettingsFile << std::endl;
        return 2;
    }

    PNGHandler imgRGB, imgDepth;
    // Get the camera resolution from the yaml settings file.cv::FileStorage fSettings(argv[2], cv::FileStorage::READ);
    imgRGB.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], fsSettings["Camera.channels"], CV_8UC3);
    imgDepth.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], 1, CV_32FC1);

    if (!stream.Init())
    {
        std::cout << "*** ERROR *** Failed to initialize pipe." << std::endl;
        try
        {
            stream.Close();
            std::cout << "## Closed hanging pipe, attempting to re-initialize..." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "*** ERROR *** Terminating program, pipe initialization failed." << std::endl;
            return 1;
        }
    }

    std::cout << "## Please run cgFastDepth/online.py for online SLAM using FastDepth estimation..." << std::endl;
    stream.OpenReadOnly();

    if (stream.fileDescriptor == -1)
    {
        std::cerr << "*** ERROR *** Failed to open the named pipe." << std::endl;
        return 1;
    }

    std::cout << std::endl;

    // Tracking runtime of live vs offline
    std::vector<float> trackingTimes;
    trackingTimes.resize(numImages);
    int i = 0;
    auto t0 = std::chrono::steady_clock::now();

    // Buffer reading
    std::vector<char> buffer;
    size_t totalBytes = imgRGB.GetSize() + (sizeof(float) * imgDepth.GetSize());
    char *tempBuffer = new char[totalBytes];
    size_t bytesRead;

    while (!stream.terminate)
    {
        bytesRead = read(stream.fileDescriptor, tempBuffer, totalBytes);
        buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        totalBytes -= bytesRead;

        if (0 == totalBytes)
        {
            if (0 == i)
            {
                t0 = std::chrono::steady_clock::now();
            }

            std::vector<char> rgbBuffer(buffer.begin(), buffer.begin() + imgRGB.GetSize());
            imgRGB.SetImageMatrixFromVector(rgbBuffer);

            std::vector<char> depthBuffer(buffer.begin() + imgRGB.GetSize(), buffer.end());
            imgDepth.SetImageMatrixFromVector(depthBuffer);

            // // If you would like to save the files
            // std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            // std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
            // imgRGB.SetFilename("Custom/rgb/" + std::to_string(timestamp) + ".png");
            // imgRGB.Save();
            // imgDepth.SetFilename("Custom/depth/" + std::to_string(timestamp) + ".tiff");
            // imgDepth.Save();

            double trackedFrame = timestamps[i];

            auto t1 = std::chrono::steady_clock::now();

            // Pass the image to the SLAM system
            SLAM.TrackRGBD(imgRGB.GetImageMatrix(), imgDepth.GetImageMatrix(), trackedFrame);

            auto t2 = std::chrono::steady_clock::now();

            rgbBuffer.clear();
            depthBuffer.clear();
            buffer.clear();
            totalBytes = imgRGB.GetSize() + (sizeof(float) * imgDepth.GetSize());

            double trackTime = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();

            trackingTimes[i] = trackTime;

            // Wait to load the next frame
            double T = 0;
            if (i < numImages - 1)
            {
                T = timestamps[i + 1] - trackedFrame;
            }
            else if (i > 0)
            {
                T = trackedFrame - timestamps[i - 1];
            }

            if (trackTime < T)
            {
                // Adjust this based on format of timestamps (use 1e-5 for TUM datasets)
                usleep((T - trackTime) * 1e-1);
            }

            ++i;
        }

        std::string bytesStr(tempBuffer, bytesRead);

        if ("terminate" == bytesStr)
        {
            stream.terminate = true;
            std::cout << "## Terminate command received, closing pipe and exiting data stream loop..." << std::endl;
            break;
        }
    }

    auto tEnd = std::chrono::steady_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::duration<double>>(tEnd - t0).count();

    SLAM.Shutdown();
    stream.Close();

    // Tracking time statistics
    sort(trackingTimes.begin(), trackingTimes.end());
    float totaltime = 0;
    for (int i = 0; i < numImages; i++)
    {
        totaltime += trackingTimes[i];
    }

    // Print time stats
    std::cout << "\n----- Timing Statistics -----" << std::endl;
    std::cout << "   Median Tracking Time (seconds): " << trackingTimes[numImages / 2] << std::endl;
    std::cout << "   Mean Tracking Time (seconds): " << totaltime / numImages << std::endl;
    std::cout << "   Execution Time (seconds): " << runtime << std::endl;

    // Save camera trajectory
    std::string filepath = "./output/FD_Estimated/";
    std::string path = argv[3];
    size_t lastSlashPos = path.find_last_of('/');
    std::string extractedPath = path.substr(12, lastSlashPos - 12);

    SLAM.SaveTrajectoryTUM(filepath + extractedPath);
    SLAM.SaveKeyFrameTrajectoryTUM(filepath + extractedPath);

    return 0;
}