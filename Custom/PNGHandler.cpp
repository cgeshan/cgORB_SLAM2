#include "PNGHandler.h"

PNGHandler::PNGHandler()
{
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->filename = "PNGHandler_Output.png";
    this->dat.resize(width * height * channels);
}
PNGHandler::PNGHandler(const int wid, const int hei, const int channels)
{
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->filename = "PNGHandler_Output.png";
    this->dat.resize(wid * hei * channels);
}

PNGHandler::~PNGHandler()
{
    CleanUp();
}

void PNGHandler::CleanUp()
{
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->dat.clear();
}

void PNGHandler::SetResolution(const int wid, const int hei, const int channels)
{
    if (wid < 1 || hei < 1)
    {
        throw std::invalid_argument("*** Error *** Image width and height must be greater than 0 pixel.");
    }
    CleanUp();
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->dat.resize(wid * hei * channels);
}

void PNGHandler::SetData(const std::vector<char> &buffer)
{
    if (this->dat != buffer)
    {
        this->dat.clear();
        this->dat = buffer;
    }
}

void PNGHandler::SetFilename(const std::string &fn)
{
    if (fn != "" && fn != this->filename)
    {
        this->filename = fn;
    }
}

int PNGHandler::GetSize(void) const
{
    return this->dat.size();
}

bool PNGHandler::Save()
{
    if (this->width > 0 && this->height > 0)
    {
        cv::Mat image(this->width, this->height, CV_8UC3);
        std::memcpy(image.data, dat.data(), dat.size());

        // Convert RGB to BGR
        cv::Mat bgrImage;
        cv::cvtColor(image, bgrImage, cv::COLOR_RGB2BGR);

        cv::imwrite(this->filename, bgrImage);

        std::cout << "Image saved with filename: " << this->filename << std::endl;
        return true;
    }
    else
    {
        throw std::invalid_argument("*** Error *** Image resolution must be greater than 1x1 pixel. Current image resolution is " + std::to_string(this->width) + " x " + std::to_string(this->height) + ". You can change the image resolution by calling SetResolution()");
        return false;
    }
}

void PNGHandler::Print()
{
    for (int i = 0; i < dat.size(); i += 3)
    {
        float r = dat[i];
        float g = dat[i + 1];
        float b = dat[i + 2];
        std::cout << "R: " << r << " G: " << g << " B: " << b << std::endl;
    }
}
