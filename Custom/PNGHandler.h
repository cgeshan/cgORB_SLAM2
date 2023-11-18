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

public:
    PNGHandler();
    PNGHandler(const int wid, const int hei, const int channels);
    void CleanUp();
    void SetResolution(const int wid, const int hei, const int channels);
    void SetData(const std::vector<char> &buffer);
    int GetSize(void) const;
    bool Save(const std::string filename);
    void Print();
};

#endif