//
// Created by Leo Qian on 7/17/22.
//
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "al/app/al_App.hpp"
#include "al/math/al_Mat.hpp"
#include "librealsense2/rs.hpp"
#include "cv-helpers.hpp"

using namespace al;

// using namespace rs2;
class WallApp : public App {
public:

// Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;

    cv::Mat image;

    WallApp() {

    }

    void onCreate() {
        nav().pos(0, 0, 0);
        nav().faceToward(Vec3d(0, 0, 1), Vec3d(0, -1, 0));

        // Configure and start the pipeline
        pipe.start();
    }

    void onAnimate(double dt) {
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Convert RealSense frame to OpenCV matrix:
//        color_mat = frame_to_mat(color);

        // Query frame size (width and height)
        const int w = color.as<rs2::video_frame>().get_width();
        const int h = color.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        cv::Mat image(cv::Size(w, h), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);

        // Update the window with new data
        imshow("window_name", image);

    }

    void onDraw(Graphics &g) {
        // Update the window with new data
//        imshow("display image", color_mat);

    }
};

int main() {
    WallApp app;
    app.start();
}


