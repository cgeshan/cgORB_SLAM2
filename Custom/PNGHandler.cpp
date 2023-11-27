#include "PNGHandler.h"

PNGHandler::PNGHandler()
{
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->filename = "PNGHandler_Output.png";
}
PNGHandler::PNGHandler(const int wid, const int hei, const int channels)
{
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->filename = "PNGHandler_Output.png";
}

PNGHandler::~PNGHandler()
{
    CleanUp();
}

void PNGHandler::CleanUp()
{
    if (nullptr != img)
    {
        delete[] img;
    }
    img = nullptr;
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

bool PNGHandler::Save() const
{
    if (0 < this->width && 0 < this->height && 0 < this->channels)
    {
        if (nullptr != this->img)
        {
            // Convert RGB to BGR
            cv::Mat bgrImage;
            cv::cvtColor(this->img, bgrImage, cv::COLOR_RGB2BGR);

            cv::imwrite(this->filename, bgrImage);

            std::cout << "Image saved with filename: " << this->filename << std::endl;
            return true;
        }
    }
    else
    {
        throw std::invalid_argument("*** Error *** Image resolution must be greater than 1x1 pixel. Current image resolution is " + std::to_string(this->width) + " x " + std::to_string(this->height) + ". You can change the image resolution by calling SetResolution()");
        return false;
    }
}
