#ifndef PNGHANDLER_H
#define PNGHANDLER_H

#include <iostream>
#include <stdexcept>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class PNGHandler
{
private:
<<<<<<< HEAD
    int width = 0, height = 0, channels = 0;
    cv::Mat *img = nullptr;
    std::string filename = "";
=======
    int width, height, channels;
    std::vector<char> dat;
    std::string filename;
    cv::Mat *img = nullptr;
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff

public:
    PNGHandler();
    PNGHandler(const int wid, const int hei, const int channels);
    ~PNGHandler();
    void CleanUp();
    void SetResolution(const int wid, const int hei, const int channels);
<<<<<<< HEAD
    void SetImageMatrix(const std::vector<char> &buffer);
    void SetFilename(const std::string &fn);
    int GetSize(void) const;
    bool Save() const;

    ImageType type;
=======
    void SetData(const std::vector<char> &buffer);
    void SetImageMatrixFromVector(const std::vector<char> &imgBuffer);
    void SetFilename(const std::string &fn);
    int GetSize(void) const;
    cv::Mat GetImageMatrix(void) const;
    bool Save();
    void Print();
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
};

#endif