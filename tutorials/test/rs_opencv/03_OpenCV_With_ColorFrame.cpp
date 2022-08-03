// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

/*
 * This code uses the file provided by intel realsense as template and show the color frame captured by the intel realsense camera
 */
#include "librealsense2/rs.hpp" // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "cv-helpers.hpp"

int main(int argc, char *argv[]) try {

    // Declare realsense config
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR);

    // Declare realsense align
    rs2::align align_to(RS2_STREAM_COLOR);


    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    using namespace cv;

    // create a window with given name
    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_NORMAL);


    while (waitKey(1) < 0 && getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0) {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera

        data = align_to.process(data);

        auto color_frame = data.get_color_frame();

        // convert frame to mat
        Mat image = frame_to_mat(color_frame);
        // Update the window with new data
        imshow(window_name, image);
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error &e) {
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    "
              << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}



