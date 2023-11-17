#ifndef AR_VR_CORE_FACE_HPP
#define AR_VR_CORE_FACE_HPP


#include <opencv2/core/mat.hpp>

class Face {
    ~Face() = default;

public:
    Face() = default;

    static cv::Mat Thresholding(const cv::Mat& img);
    static cv::Mat Contour(cv::Mat & Image);
};


#endif //AR_VR_CORE_FACE_HPP
