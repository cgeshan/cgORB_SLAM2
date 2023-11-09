#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <chrono>

#include <opencv2/core/core.hpp>

// #include <System.h>

#include "DataStream.h"

std::vector<float> ExtractRGBD(std::vector<float> &data, const char *buffer)
{
    std::string received(buffer);
    std::istringstream ss(received);
    while (std::getline(ss, received, ' ') && strcmp(buffer, "terminate") != 0)
    {
        data.push_back(std::stof(received));
    }
    return data;
}

void PrintRGBD(std::vector<float> data)
{
    for (int idx = 0; idx < data.size(); idx += 4)
    {
        float r = data[idx];
        float g = data[idx + 1];
        float b = data[idx + 2];
        float d = data[idx + 3];
        std::cout << "R: " << r << " G: " << g << " B: " << b << " D: " << d << std::endl;
    }
}

int main()
{
    DataStream stream;
    std::vector<float> data;

    if (!stream.Init("data_stream", 0666))
    {
        std::cout << "Terminating" << std::endl;
        return 1;
    }

    stream.OpenReadOnly();

    if (stream.fileDescriptor == -1)
    {
        std::cerr << "Failed to open the named pipe." << std::endl;
        return 1;
    }

    while (true)
    {
        char buffer[100];
        ssize_t bytes_read = read(stream.fileDescriptor, buffer, sizeof(buffer));

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            std::cout << "Received: " << buffer << std::endl;
            ExtractRGBD(data, buffer);
        }

        if (strcmp(buffer, "terminate") == 0)
        {
            break;
        }
    }

    stream.Close();

    PrintRGBD(data);

    return 0;
}
