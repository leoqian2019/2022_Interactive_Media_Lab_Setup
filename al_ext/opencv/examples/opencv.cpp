#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"

#include "al_ext/opencv/al_OpenCV.hpp"
#include "opencv2/objdetect.hpp"

using namespace al;

class MyApp : public App {
public:
  // Simplified OpenCV functions
  AlloOpenCV ocv;

  cv::CascadeClassifier faces_classifier;
  std::vector<cv::Rect> faces;
  //  cv::CascadeClassifier eyes;

  void onCreate() override {
    addCube(mCube);
    mCube.primitive(Mesh::LINE_STRIP);
    mCube.update();

    // From openCV documentation
    std::string face_cascade_name = "../../opencv/sources/data/haarcascades/"
                                    "haarcascade_frontalface_alt.xml";
    std::string eyes_cascade_name = "../../opencv/sources/data/haarcascades/"
                                    "haarcascade_eye_tree_eyeglasses.xml";

    if (!faces_classifier.load(face_cascade_name)) {
      printf("--(!)Error loading face cascade\n");
    };
    if (!faces_classifier.load(eyes_cascade_name)) {
      printf("--(!)Error loading eyes cascade\n");
    };

    ocv.initializeVideoCaptureDevice();
    //    cv.initializeVideoCaptureFile(
    //        "path/to/video");
  }

  void onAnimate(double dt) override {

    ocv.captureFrameIntoTexture();

    { // Find faces
      cv::Mat frame_gray;

      cvtColor(ocv.videoImage, frame_gray, cv::COLOR_BGR2GRAY);
      equalizeHist(frame_gray, frame_gray);

      //-- Detect faces
      faces_classifier.detectMultiScale(frame_gray, faces, 1.1, 2,
                                        0 | cv::CASCADE_SCALE_IMAGE,
                                        cv::Size(30, 30));
    }
  }

  void onDraw(Graphics &g) override {
    g.clear(0.3);

    g.pushMatrix();
    g.translate(0, 0, -5);

    g.quad(ocv.videoTexture, -1 * ocv.frameAspect(), -1, 2 * ocv.frameAspect(),
           2, true);
    for (const auto &face : faces) {
      Mesh m(Mesh::LINE_STRIP);
      m.vertex(face.x, face.y);
      m.vertex(face.x, face.y + face.height);
      m.vertex(face.x + face.width, face.y + face.height);
      m.vertex(face.x + face.width, face.y);
      m.vertex(face.x, face.y);
      g.color(0, 1.0, 0);

      {
        g.pushMatrix();
        g.translate(1, 1, 0);
        g.scale(-1.0 / ocv.videoTexture.width(),
                -1.0 / ocv.videoTexture.height(), 1.0);
        g.scale(2.0);
        g.draw(m);
        g.popMatrix();
      }
    }

    g.popMatrix();
  }

  void onExit() override {}

private:
  VAOMesh mCube;
};

int main() {
  MyApp app;

  app.start();
  return 0;
}
