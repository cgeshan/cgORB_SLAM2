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
    int width = 0, height = 0, channels = 0;
    std::string filename = "";
    std::vector<char> dat = {};
    cv::Mat *img = nullptr;

public:
    PNGHandler();
    PNGHandler(const int wid, const int hei, const int channels, int cvType);
    ~PNGHandler();
    void CleanUp();
    void SetResolution(const int wid, const int hei, const int channels, int cvType);
    void SetImageMatrixFromVector(const std::vector<char> &buffer);
    void SetFilename(const std::string &fn);
    int GetSize(void) const;
    cv::Mat GetImageMatrix(void) const;
    bool Save() const;
};

#endif