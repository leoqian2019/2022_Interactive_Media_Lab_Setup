This is a copy of the repo: https://github.com/AlloSphere-Research-Group/allolib_playground with new modules added to
expand the use case of Allolib Application

Here's the part of the read me file from the original repo which helps you to set up the allolib_playground application

# 2022_Interactive_Media_Lab_Setup

This repo provides a quick and simple way to build allolib applications. It also contains a set of tutorials and
cookbook examples in addition to the examples in the allolib library.

## Setup

Get all dependencies and tools required by allolib. See
the [readme file for allolib](https://github.com/AlloSphere-Research-Group/allolib/blob/master/readme.md).

### Add to include folder

In order for hpp files to be recognized by IDEs like Clion, they have to be added to allolib/include folder

At the same time, you have to install dependencies:
librealsense2: https://github.com/IntelRealSense/librealsense (allows you to use the depth camera)
opencv: (use brew to install)

At the same time, make sure you put the folder "librealsense2" and its content from
https://github.com/IntelRealSense/librealsense/tree/master/include into the "include" folder of this repo as well

### Modify the CMakeList.txt

Since we use run.sh to build this complex application, we need to modify allolib/CMakeList.txt in order for allolib to
accept the external dependencies

More Specifically, it's line 349 that you guys need to modify. By default, it will expect you guys have librealsense2
and OpenCV installed

### Fix for m1 user

We found a solution so that Allolib would run on your macs. This meant making modifications to the conditionals in the
following files in allolib/external/oscpack/osc folder

oscType.h line 64

oscReceiveElements.h line 103

oscOutBoundPacketStream.h line 108

oscType.h line 64

switch `#if defined(x86_64) || defined(_M_X64)` to `#if (defined(x86_64) || defined(_M_X64) || true)`

and on oscReceiveElements.h line 103 and oscOutBoundPacketStream.h line 108

switch `#if !(defined(x86_64) || defined(_M_X64) )` to `#if !(defined(x86_64) || defined(_M_X64) || true)`

## Building and running applications

The allolib playground provides basic facilties for building and debugging single file applications. On a bash shell on
Windows, Linux and OS X do:

    ./run.sh path/to/file.cpp

This will build allolib, and create an executable for the file.cpp called 'file' inside the '''path/to/bin''' directory.
It will then run the application.

### Add more files

The files created by myself are all stored in tutorials/test. You guys can feel free to add more files and use run.sh to
build and test them

### Demo files

For the purpose of testing what we know about allolib, intel realsense, and opencv, we created several demo applications
all located in tutorial/test folder

1. 01_Box.cpp: creates a blue rotating box
2. 01_Sphere.cpp: creates a rotating sphere with two quads around it
3. 02_PointCloud: draw the 3d space cpatured by the camera using allolib
4. rs_opencv/03_OpenCV_With_ColorFrame.cpp: using opencv to display color frames of the camera
5. rs_opencv/03_OpenCV_With_BG_Subtraction_Color.cpp: running the default background subtractor of opencv on the color
   frame of the camera
6. rs_opencv/03_OpenCV_With_BG_Subtraction_Dpeth.cpp: running the default background subtractor using the depth frame

In the rs_opencv folder, there's a `run_opencv.sh` file which by putting the cpp file name without `.cpp`, you will be
able to get an executable with the same name as the file as long as realsense and opencv are installed correctly
