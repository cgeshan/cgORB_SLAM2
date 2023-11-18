#include "DataStream.h"

DataStream::DataStream()
{
    this->filename = nullptr;
    this->permission = 0644;
    this->terminate = false;
}

DataStream::DataStream(const char *path, mode_t mode)
{
    this->filename = path;
    this->permission = mode;
}

DataStream::~DataStream()
{
    if (nullptr != filename)
    {
        // Close();
        if (unlink(filename) == 0)
        {
            std::cout << "## Program terminated, data stream pipe deleted successfully." << std::endl;
        }
        else
        {
            std::cerr << "*** ERROR *** Unable to delete data stream pipe. Ensure you have closed the data stream first. (stream.Close())" << std::endl;
        }

        permission = 0000;
    }
}

bool DataStream::Init()
{

    if (mkfifo(filename, permission) == 0)
    {
        std::cout << "## Data stream created..." << std::endl;
        this->terminate = false;
        return true;
    }
    else
    {
        std::cerr << "*** ERROR *** Unable to create data stream pipe" << std::endl;
        return false;
    }
}

void DataStream::SetFileName(const char *filename)
{
    this->filename = filename;
}

void DataStream::SetPermission(const mode_t mode)
{
    this->permission = mode;
}

std::string DataStream::GetFileName()
{
    if (filename != nullptr)
    {
        return std::string(filename);
    }
    else
    {
        return "No filename set";
    }
}

int DataStream::OpenReadOnly()
{
    fileDescriptor = open(filename, O_RDONLY);
    if (fileDescriptor == -1)
    {
        std::cerr << "*** ERROR *** Error opening the named pipe for reading." << std::endl;
    }
    else
    {
        std::cout << "## Opening in read only mode... " << std::endl;
    }

    return fileDescriptor;
}

int DataStream::OpenWriteOnly()
{
    fileDescriptor = open(filename, O_WRONLY);
    if (fileDescriptor == -1)
    {
        std::cerr << "*** ERROR *** Error opening the named pipe for writing." << std::endl;
    }
    else
    {
        std::cout << "## Opening in write only mode... " << std::endl;
    }

    return fileDescriptor;
}

int DataStream::OpenReadWrite()
{
    fileDescriptor = open(filename, O_RDWR);
    if (fileDescriptor == -1)
    {
        std::cerr << "*** ERROR *** Error opening the named pipe." << std::endl;
    }
    else
    {
        std::cout << "## Opening in read and write mode... " << std::endl;
    }

    return fileDescriptor;
}

void DataStream::Close()
{
    if (close(fileDescriptor) == 0)
    {
        std::cout << "## Named pipe closed successfully." << std::endl;
    }
    else
    {
        std::cerr << "*** ERROR *** Error closing the named pipe." << std::endl;
    }
}
