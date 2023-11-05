## 1. CMake

```bash
  sudo apt-get install cmake
```

## 2. OpenCV

### Ubuntu 22.04

```bash
  sudo apt install build-essential git pkg-config libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev gfortran openexr libatlas-base-dev python3-dev python3-numpy libtbb2 libtbb-dev libdc1394-dev
```

### Older LTS

```bash
  sudo apt install build-essential git pkg-config libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev gfortran openexr libatlas-base-dev python3-dev python3-numpy libtbb2 libtbb-dev libdc1394-22-dev
```

### Before proceeding, ensure g++ was successfuilly installed by running:

```bash
  which g++ && g++ --version
```

```bash
  git clone https://github.com/opencv/opencv.git
  git clone https://github.com/opencv/opencv_contrib.git
```

```bash
  cd opencv
  git checkout 3.4.9

  cd ../opencv_contrib
  git checkout 3.4.9

  cd ../opencv
  mkdir build && cd build
  cmake -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
  make -j12

  sudo make install
```

## 3. Eigen3

```bash
  sudo apt install libeigen3-dev
```

## 4. Pangolin

```bash
  git clone --recursive https://github.com/stevenlovegrove/Pangolin.git
  cd Pangolin

  ./scripts/install_prerequisites.sh recommended

  git checkout v0.5
```

### Please make the following changes to the Pangolin source code before attempting to build.

- Pangolin/CMakeModules/FindFFMPEG.cmake

Replace lines 63, 64 with:

```code
  sizeof(AVFormatContext::max_analyze_duration);
  }" HAVE_FFMPEG_MAX_ANALYZE_DURATION
```

- Pangolin/src/video/drivers/ffmpeg.cpp

Please add this above "namespace pangolin" (line 37)

```code
  #define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER
```

Replace lines 78, 79 with:

```code
  #ifdef FF_API_XVMC
    TEST_PIX_FMT_RETURN(XVMC_MPEG2_MC);
    TEST_PIX_FMT_RETURN(XVMC_MPEG2_IDCT);
  #endif
```

Replace lines 101 - 105 with:

```code
  #ifdef FF_API_VDPAU
    TEST_PIX_FMT_RETURN(VDPAU_H264);
    TEST_PIX_FMT_RETURN(VDPAU_MPEG1);
    TEST_PIX_FMT_RETURN(VDPAU_MPEG2);
    TEST_PIX_FMT_RETURN(VDPAU_WMV3);
    TEST_PIX_FMT_RETURN(VDPAU_VC1);
  #endif
```

Replace line 127 with:

```code
  #ifdef FF_API_VDPAU
    TEST_PIX_FMT_RETURN(VDPAU_MPEG4);
  #endif
```

- Pangolin/include/pangolin/video/drivers/ffmpeg.h

Please add this above "namespace pangolin" (line 53)

```code
  #define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
  #define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER
  #define AVFMT_RAWPICTURE 0x0020
```

- Pangolin/src/display/device/display_x11.cpp

Replace line 115 with:

```code
  // GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
```

Replace line 177 with:

```code
  // throw std::runtime_error("Pangolin in X11: ...");
```

- Pangolin/src/CMakeLists.txt

Replace lines 287-295 with:

```code
  # find_package(FFMPEG QUIET)
  # if(BUILD_PANGOLIN_VIDEO AND FFMPEG_FOUND)
  #   set(HAVE_FFMPEG 1)
  #   list(APPEND INTERNAL_INC  ${FFMPEG_INCLUDE_DIRS} )
  #   list(APPEND LINK_LIBS ${FFMPEG_LIBRARIES} )
  #   list(APPEND HEADERS ${INCDIR}/video/drivers/ffmpeg.h)
  #   list(APPEND SOURCES video/drivers/ffmpeg.cpp)
  #   message(STATUS "ffmpeg Found and Enabled")
  # endif()
```

### Now proceed to build and install Pangolin

```bash
  mkdir build && cd build
  cmake build ..
  cmake --build .. -DCMAKE_CXX_COMPILER=g++
  sudo make install
```

## Build cgORB_SLAM2

#### Initial Build

```bash
  cd cgORB_SLAM2
  chmod +x build.sh
  ./build.sh
```

#### Subsequent Builds

```bash
  cd build
  cmake --build .
```
