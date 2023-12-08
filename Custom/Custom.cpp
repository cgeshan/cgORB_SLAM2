#include <algorithm>
#include <chrono>
#include <tiffio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <System.h>

#include "DataStream.h"
#include "PNGHandler.h"

void LoadTimeStamps(const string &strFile, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    while (!f.eof())
    {
        string s;
        getline(f, s);
        if (!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            ss >> t;
            vTimestamps.push_back(t);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        cerr << endl
             << "* Improper Usage * ./Custom path_to_vocabulary path_to_camera_settings path_to_txt" << endl;
        return 1;
    }

    vector<double> vTimestamps;
    string strFile = string(argv[3]) + "/timestamps.txt";
    LoadTimeStamps(strFile, vTimestamps);
    int nImages = vTimestamps.size();

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
    imgDepth.SetResolution(fsSettings["Camera.width"], fsSettings["Camera.height"], 1, CV_32FC1);

    if (!stream.Init())
    {
        std::cout << "*** ERROR *** Terminating program, pipe initialization failed." << std::endl;
        return 1;
    }

    std::cout << "## Please run cgFastDepth/online.py for online SLAM using FastDepth estimation..." << std::endl;
    stream.OpenReadOnly();

    if (stream.fileDescriptor == -1)
    {
        std::cerr << "Failed to open the named pipe." << std::endl;
        return 1;
    }

    // Tracking runtime of live vs offline
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);
    int i = 0;
    auto start = std::chrono::high_resolution_clock::now();

    // Buffer reading
    std::vector<char> buffer;
    size_t totalBytes = imgRGB.GetSize() + (sizeof(float) * imgDepth.GetSize());
    char tempBuffer[totalBytes];
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
                start = std::chrono::high_resolution_clock::now();
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

            double tframe = vTimestamps[i];

#ifdef COMPILEDWITHC11
            std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
            std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

            // Pass the image to the SLAM system
            SLAM.TrackRGBD(imgRGB.GetImageMatrix(), imgDepth.GetImageMatrix(), tframe);

#ifdef COMPILEDWITHC11
            std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
            std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

            rgbBuffer.clear();
            depthBuffer.clear();
            buffer.clear();
            totalBytes = imgRGB.GetSize() + (sizeof(float) * imgDepth.GetSize());

            double ttrack = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();

            vTimesTrack[i] = ttrack;

            // Wait to load the next frame
            double T = 0;
            if (i < nImages - 1)
                T = vTimestamps[i + 1] - tframe;
            else if (i > 0)
                T = tframe - vTimestamps[i - 1];

            if (ttrack < T)
                usleep((T - ttrack) * 1e-5);

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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    SLAM.Shutdown();
    stream.Close();

    // Tracking time statistics
    sort(vTimesTrack.begin(), vTimesTrack.end());
    float totaltime = 0;
    for (int ni = 0; ni < nImages; ni++)
    {
        totaltime += vTimesTrack[ni];
    }
    cout << "-------" << endl
         << endl;
    cout << "median tracking time: " << vTimesTrack[nImages / 2] << endl;
    cout << "mean tracking time: " << totaltime / nImages << endl;

    // Save camera trajectory
    std::string filepath = "./output/RGB_D/";
    std::string path = argv[3];
    size_t lastSlashPos = path.find_last_of('/');
    std::string extractedPath = path.substr(12, lastSlashPos - 12);

    SLAM.SaveTrajectoryTUM(filepath + extractedPath);
    SLAM.SaveKeyFrameTrajectoryTUM(filepath + extractedPath);

    return 0;
}