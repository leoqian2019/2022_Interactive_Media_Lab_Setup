This is a copy of the repo: https://github.com/AlloSphere-Research-Group/allolib_playground with new modules added to
expand the use case of Allolib Application

Here's the part of the read me file from the original repo which helps you to set up the allolib_playground application

# Allolib playground

[![Build Status](https://travis-ci.org/AlloSphere-Research-Group/allolib_playground.svg?branch=master)](https://travis-ci.org/AlloSphere-Research-Group/allolib_playground)

This repo provides a quick and simple way to build allolib applications. It also contains a set of tutorials and
cookbook examples in addition to the examples in the allolib library.

## Setup

Get all dependencies and tools required by allolib. See
the [readme file for allolib](https://github.com/AlloSphere-Research-Group/allolib/blob/master/readme.md).

## Building and running applications

The allolib playground provides basic facilties for building and debugging single file applications. On a bash shell on
Windows, Linux and OS X do:

    ./run.sh path/to/file.cpp

This will build allolib, and create an executable for the file.cpp called 'file' inside the '''path/to/bin''' directory.
It will then run the application.

## Add to include folder

In order for hpp files to be recognized by IDEs like Clion, they have to be added to allolib/include folder

At the same time, you have to install dependencies:
librealsense2: https://github.com/IntelRealSense/librealsense (allows you to use the depth camera)
opencv: (use brew to install)

## Modify the CMakeList.txt

Since we use run.sh to build this complex application, we need to modify allolib/CMakeList.txt in order for allolib to
accept the external dependencies

More Specifically, it's line 349 that you guys need to modify. By default, it will expect you guys have librealsense2
and OpenCV installed

## Add more files

The files created by myself are all stored in tutorials/test. You guys can feel free to add more files and use run.sh to
build and test them

