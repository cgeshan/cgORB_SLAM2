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
    int width, height, channels;
    std::vector<char> dat;
    std::string filename;
    cv::Mat *img = nullptr;

public:
    PNGHandler();
    PNGHandler(const int wid, const int hei, const int channels);
    ~PNGHandler();
    void CleanUp();
    void SetResolution(const int wid, const int hei, const int channels);
    void SetImageMatrixFromVector(const std::vector<char> &imgBuffer);
    void SetFilename(const std::string &fn);
    int GetSize(void) const;
    cv::Mat GetImageMatrix(void) const;
    bool Save();
};

#endif