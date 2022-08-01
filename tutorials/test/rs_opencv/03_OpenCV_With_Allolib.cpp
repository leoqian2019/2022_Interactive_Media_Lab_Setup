#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "librealsense2/rs.hpp" // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "cv-helpers.hpp"


using namespace al;

class WallApp : public App {
public:

    // mesh to store the points we're rendering
    Mesh verts;

    // Declare realsense config
    rs2::config cfg;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;

    WallApp() {
    }

    void onCreate() {
        cfg.enable_stream(RS2_STREAM_COLOR);
        // Configure and start the pipeline
        pipe.start();
    }


    void onAnimate(double dt) {
        // Declare realsense align
        rs2::align align_to(RS2_STREAM_COLOR);
        // get frames
        auto frames = pipe.wait_for_frames();
        // align the frame received
        frames = align_to.process(frames);
        // get color frames
        auto color = frames.get_color_frame();


        // convert frame to mat
        cv::Mat image = frame_to_mat(color);
        // Update the window with new data
        imshow("Alloapp", image);


    }

    void onDraw(Graphics &g) {

    }
};

int main() {
    WallApp app;
    app.start();
}
