#ifndef INCLUDE_OPENCV_HPP
#define INCLUDE_OPENCV_HPP

#include "al/app/al_App.hpp"
#include "al/graphics/al_Graphics.hpp"
#include "al/graphics/al_Texture.hpp"

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <functional>
#include <iostream>

namespace al {

class AlloOpenCV {
public:
  std::unique_ptr<cv::VideoCapture> videoCapture;
  cv::Mat videoImage;
  al::Texture videoTexture;

  AlloOpenCV() {}
  virtual ~AlloOpenCV() {}

  ///////////////// Video capture
  // video capture
  void initializeVideoCaptureDevice(int index = 0, bool initTexture = true) {
    videoCapture = std::make_unique<cv::VideoCapture>(index);
    if (initTexture) {
      intializeTextureFromVideoCapture();
    }
  }

  // Urls are allowed
  void initializeVideoCaptureFile(std::string filename,
                                  bool initTexture = true) {
    videoCapture = std::make_unique<cv::VideoCapture>(filename);
    if (initTexture) {
      intializeTextureFromVideoCapture();
    }
  }

  void cleanupVideoCapture() { videoCapture = nullptr; }

  // Video capturing
  void captureFrameIntoTexture() {
    captureFrame();
    videoTexture.submit(videoImage.ptr());
  }

  void captureFrame() { *videoCapture >> videoImage; }

  float frameAspect() {
    return videoTexture.width() / (float)videoTexture.height();
  }

protected:
  void intializeTextureFromVideoCapture() {

    auto width = videoCapture->get(cv::CAP_PROP_FRAME_WIDTH);
    auto height = videoCapture->get(cv::CAP_PROP_FRAME_HEIGHT);
    // TODO use fps
    auto fps = videoCapture->get(cv::CAP_PROP_FPS);
    if (videoTexture.created()) {
      videoTexture.destroy();
      std::cout << "Destroying texture before creating new one" << std::endl;
    }

    //      cv::CAP_PROP_MONOCHROME
    videoTexture.create2D(width, height, Texture::RGB8, Texture::BGR,
                          Texture::UBYTE);
    std::cout << "Created texture " << width << "x" << height << std::endl;

    videoTexture.filter(Texture::LINEAR);
    videoTexture.generateMipmap();
  }
};

} // namespace al

#endif
