#include "PNGHandler.h"

PNGHandler::PNGHandler()
{
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->filename = "PNGHandler_Output.png";
<<<<<<< HEAD
<<<<<<< HEAD
=======
    this->dat.resize(width * height * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
=======
    this->dat.resize(width * height * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
}
PNGHandler::PNGHandler(const int wid, const int hei, const int channels)
{
    this->width = wid;
    this->height = hei;
    this->channels = channels;
    this->filename = "PNGHandler_Output.png";
<<<<<<< HEAD
<<<<<<< HEAD
=======
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
=======
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
}

PNGHandler::~PNGHandler()
{
    CleanUp();
}

void PNGHandler::CleanUp()
{
<<<<<<< HEAD
<<<<<<< HEAD
    if (nullptr != img)
    {
        delete[] img;
    }
    img = nullptr;
=======
=======
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
    this->width = 0;
    this->height = 0;
    this->channels = 0;
    this->dat.clear();
    this->img = nullptr;
<<<<<<< HEAD
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
=======
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
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
<<<<<<< HEAD
<<<<<<< HEAD
=======
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
=======
    this->dat.resize(wid * hei * channels);
    this->img = new cv::Mat(this->width, this->height, CV_8UC3);
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
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

<<<<<<< HEAD
<<<<<<< HEAD
bool PNGHandler::Save() const
=======
=======
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
cv::Mat PNGHandler::GetImageMatrix(void) const
{
    return *img;
}

bool PNGHandler::Save()
<<<<<<< HEAD
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
{
    if (0 < this->width && 0 < this->height && 0 < this->channels)
    {
<<<<<<< HEAD
        if (nullptr != this->img)
        {
            // Convert RGB to BGR
            cv::Mat bgrImage;
            cv::cvtColor(this->img, bgrImage, cv::COLOR_RGB2BGR);

            cv::imwrite(this->filename, bgrImage);

            std::cout << "Image saved with filename: " << this->filename << std::endl;
            return true;
        }
=======
=======
{
    if (0 < this->width && 0 < this->height && 0 < this->channels)
    {
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
        cv::Mat image(this->height, this->width, CV_8U);
        std::memcpy(image.data, dat.data(), dat.size());

        // Convert RGB to BGR
        // cv::Mat bgrImage;
        // cv::cvtColor(image, bgrImage, cv::COLOR_RGB2BGR);

        cv::imwrite(this->filename, image);

        std::cout << "Image saved with filename: " << this->filename << std::endl;
        return true;
<<<<<<< HEAD
>>>>>>> 9b788acf309b6c08670892108cfcdf168eef2bff
=======
>>>>>>> 3321a883e5651259f82ccb4e61777865cd3842e7
    }
    else
    {
        throw std::invalid_argument("*** Error *** Image resolution must be greater than 1x1 pixel. Current image resolution is " + std::to_string(this->width) + " x " + std::to_string(this->height) + ". You can change the image resolution by calling SetResolution()");
        return false;
    }
}
