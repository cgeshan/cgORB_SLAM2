#include "PNGHandler.h"

PNGHandler::PNGHandler()
{
}
PNGHandler::PNGHandler(const int wid, const int hei, const int channels, int cvType)
{
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->filename = "PNGHandler_Output.png";
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->height, this->width, cvType);
}

PNGHandler::~PNGHandler()
{
}

void PNGHandler::CleanUp()
{
    if (nullptr != img)
    {
        delete[] img;
    }
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->dat.clear();
    this->img = nullptr;
}

void PNGHandler::SetResolution(const int wid, const int hei, const int channels, int cvType)
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
    this->img = new cv::Mat(this->height, this->width, cvType);
}

void PNGHandler::SetImageMatrixFromVector(const std::vector<char> &imgBuffer)
{
    if (!imgBuffer.empty())
    {
        std::copy(imgBuffer.begin(), imgBuffer.end(), this->img->data);
    }
}

void PNGHandler::SetFilename(const std::string &fn)
{
    if (fn != "" && fn != this->filename)
    {
        this->filename = fn;
        std::cout << "## Image filename has been set to: " << fn << std::endl;
    }
}

int PNGHandler::GetSize(void) const
{
    return this->width * this->height * this->channels;
}

cv::Mat PNGHandler::GetImageMatrix(void) const
{
    return *this->img;
}

bool PNGHandler::Save() const
{
    if (0 < this->width && 0 < this->height && 0 < this->channels)
    {
        if (nullptr != this->img)
        {
            // Convert RGB to BGR
            if (3 == channels)
            {
                cv::Mat bgrImage;
                cv::cvtColor(*this->img, bgrImage, cv::COLOR_RGB2BGR);

                cv::imwrite(this->filename, bgrImage);
            }
            else
            {
                cv::imwrite(this->filename, *this->img);
            }

            std::cout << "## Image saved with filename: " << this->filename << std::endl;
            return true;
        }

        else
        {
            throw std::invalid_argument("*** Error *** Image resolution must be greater than 1x1 pixel. Current image resolution is " + std::to_string(this->width) + " x " + std::to_string(this->height) + ". You can change the image resolution by calling SetResolution()");
            return false;
        }
    }
}