#!/bin/bash
g++ -std=c++11 $1 -o out  -lrealsense2 -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video
./out
