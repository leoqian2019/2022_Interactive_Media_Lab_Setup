# setting min version
cmake_minimum_required(VERSION 3.1.0)

PROJECT(rs_opencv)

find_package(OpenCV REQUIRED)
set(DEPENDENCIES realsense2 $(OpenCV_LIBS))

message(STATUS "OPENCV VERSION $(OpenCV_VERSION)")

add_executable(rs_dnn rs_dnn.cpp include/cv-helpers.hpp)

set_property(TARGET rs_opencv PROPERTY CXX_STANDARD 11)

target_link_libraries(rs_opencv $(DEPENDENCIES))