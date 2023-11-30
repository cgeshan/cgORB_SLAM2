#include "PNGHandler.h"

PNGHandler::PNGHandler()
{
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->filename = "PNGHandler_Output.png";
    this->dat.resize(width * height * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
}
PNGHandler::PNGHandler(const int wid, const int hei, const int channels)
{
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->filename = "PNGHandler_Output.png";
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
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
    this->img = nullptr;
}

void PNGHandler::SetResolution(const int wid, const int hei, const int channels)
{
    if (1 > wid || 1 > hei || 1 > channels)
    {
        throw std::invalid_argument("*** Error *** Image width and height must be greater than 0 pixel.");
    }
    CleanUp();
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
}

void PNGHandler::SetImageMatrix(const std::vector<char> &buffer)
{
    if (!buffer.empty())
    {
        if (nullptr != this->img)
        {
            CleanUp();
        }
        this->img = new Mat(this->width, this->height, CV_8UC3);
        std::memcpy(img.data, buffer.data(), buffer.size());
    }
    else
    {
        throw std::invalid_argument("*** Error *** Unable to set image from buffer, buffer was empty...");
    }
}

void PNGHandler::SetImageMatrixFromVector(const std::vector<char> &imgBuffer)
{
    if (!imgBuffer.empty())
    {
        std::copy(imgBuffer.begin(), imgBuffer.end(), img->data);
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
    return this->width * this->height * this->channels;
}

cv::Mat PNGHandler::GetImageMatrix(void) const
{
    return *img;
}

bool PNGHandler::Save()
{
    if (0 < this->width && 0 < this->height && 0 < this->channels)
    {
        cv::Mat image(this->height, this->width, CV_8U);
        std::memcpy(image.data, dat.data(), dat.size());

        // Convert RGB to BGR
        // cv::Mat bgrImage;
        // cv::cvtColor(image, bgrImage, cv::COLOR_RGB2BGR);

        cv::imwrite(this->filename, image);

        std::cout << "Image saved with filename: " << this->filename << std::endl;
        return true;
    }
    else
    {
        throw std::invalid_argument("*** Error *** Image resolution must be greater than 1x1 pixel. Current image resolution is " + std::to_string(this->width) + " x " + std::to_string(this->height) + ". You can change the image resolution by calling SetResolution()");
        return false;
    }
}
