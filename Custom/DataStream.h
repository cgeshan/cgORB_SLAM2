#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

class DataStream
{
private:
    const char *filename;
    mode_t permission;

public:
    DataStream();
    ~DataStream();
    bool Init(const char *filename, mode_t mode);
    void SetFileName(const char *filename);
    void SetPermission(const mode_t mode);
    std::string GetFileName();
    int OpenReadOnly();
    int OpenWriteOnly();
    int OpenReadWrite();
    void Close();

    int fileDescriptor;
};

#endif