#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "librealsense2/rs.hpp" // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "cv-helpers.hpp"

using namespace cv;
using namespace std;
const char *params
        = "{ help h         |           | Print usage }"
          "{ input          | vtest.avi | Path to a video or a sequence of image }";

int main(int argc, char *argv[]) {

    // handle parameter related to background subtraction
    CommandLineParser parser(argc, argv, params);
    parser.about("This program shows how to use background subtraction methods provided by "
                 " OpenCV. You can process both videos and images.\n");
    if (parser.has("help")) {
        //print help information
        parser.printMessage();
    }
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2") {
        pBackSub = createBackgroundSubtractorMOG2();
    } else {
        pBackSub = createBackgroundSubtractorKNN();
    }

    // handle realsense camera api
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


    Mat frame, fgMask;
    while (true) {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera

        data = align_to.process(data);

        auto color_frame = data.get_color_frame();
        // convert frame to mat
        frame = frame_to_mat(color_frame);

        //update the background model
        pBackSub->apply(frame, fgMask);

        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);
        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    return 0;
}
