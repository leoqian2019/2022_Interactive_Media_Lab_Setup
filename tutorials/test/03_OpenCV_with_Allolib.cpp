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
#include "al_ext/opencv/al_OpenCV.hpp"

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/math/al_Mat.hpp"
#include <librealsense2/rs.hpp>
#include "librealsense/wrappers/opencv/cv-helpers.hpp"

using namespace al;

// using namespace rs2;
class WallApp : public App {
public:

// Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;

    cv::Mat color_mat;


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
        color_mat = frame_to_mat(color);


    }

    void onDraw(Graphics &g) {
        // Update the window with new data
        imshow("display image", color_mat);

    }
};

int main() {
    WallApp app;
    app.start();
}


