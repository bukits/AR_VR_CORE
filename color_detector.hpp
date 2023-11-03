#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class ColorDetector {
public:
    ColorDetector() = default;

    virtual ~ColorDetector() = default;

    // inout from the FE BZS
    void LoadImages();
    void FaceDetection();

    static void StoreColors(cv::Mat& image, cv::Mat& colorMatrix);

    //Top, left, front, right, back, bottom is the order from row processing from left to right
    // only for one face
    static std::string CreateCubeState(cv::Mat& colorMatrix);
};
