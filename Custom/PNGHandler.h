#ifndef PNGHANDLER_H
#define PNGHANDLER_H

#include <iostream>
#include <stdexcept>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

enum ImageType
{
    DEPTH_IMAGE,
    RGB_IMAGE
};

class PNGHandler
{
private:
    int width = 0, height = 0, channels = 0;
    cv::Mat *img = nullptr;
    std::string filename = "";

public:
    PNGHandler();
    PNGHandler(const int wid, const int hei, const int channels);
    ~PNGHandler();
    void CleanUp();
    void SetResolution(const int wid, const int hei, const int channels);
    void SetImageMatrix(const std::vector<char> &buffer);
    void SetFilename(const std::string &fn);
    int GetSize(void) const;
    bool Save() const;

    ImageType type;
};

#endif