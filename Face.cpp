#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include "Face.hpp"



cv::Mat Face::Thresholding(const cv::Mat& Image) {
    cv::Mat modified;
    cv::GaussianBlur(Image, modified, cv::Size(0, 0), 5);
    cv::addWeighted(Image, 4, modified, -0.8, 0, modified);

    cv::cvtColor(modified, modified, cv::COLOR_BGR2GRAY);

    cv::threshold(modified, modified, 100, 150, cv::THRESH_BINARY);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(50, 50));
    cv::Mat closedImage;
    cv::morphologyEx(modified, closedImage, cv::MORPH_OPEN, kernel);


    // cv::imshow("closedImage Image", closedImage);

    //cv::waitKey(0);
    return closedImage;
}
cv::Mat ConvertToBinary(const cv::Mat& image) {
    cv::Mat binaryImage;
    cv::threshold(image, binaryImage, 100, 1, cv::THRESH_BINARY);
    return binaryImage;
}
cv::Mat Face::Contour(cv::Mat &Image) {
    cv::Mat Original = Image;
    Image = ConvertToBinary(Image);
    int top = 0, bottom = Image.rows - 1, left = 0, right = Image.cols - 1;

    double thresholdValue = 0.8; // THRESHOLD


    // TOP
    for (int y = 0; y < Image.rows; ++y) {
        int rowSum = cv::sum(Image.row(y))[0];
        // std::cout << "Row " << y << " sum: " << rowSum << " VALUE: " <<thresholdValue * Image.cols << std::endl;
        if (rowSum < thresholdValue * Image.cols) {
            top = y;
            break;
        }
    }
    // std::cout << "Top Coordinate: (" << left << ", " << top << ")" << std::endl;

    // BOTTOM
    for (int y = Image.rows - 1; y >= 0; --y) {
        int rowSum = cv::sum(Image.row(y))[0];
        // std::cout << "Row " << y << " sum: " << rowSum << std::endl;
        if (rowSum < thresholdValue * Image.cols) {
            bottom = y;
            break;
        }
    }
    // std::cout << "Bottom Coordinate: (" << left << ", " << bottom << ")" << std::endl;

    // LEFT
    for (int x = 0; x < Image.cols; ++x) {
        int colSum = cv::sum(Image.col(x))[0];
        // std::cout << "Column " << x << " sum: " << colSum << std::endl;
        if (colSum < thresholdValue * Image.rows) {
            left = x;
            break;
        }
    }
    // std::cout << "Left Coordinate: (" << left << ", " << top << ")" << std::endl;

    // RIGHT
    for (int x = Image.cols - 1; x >= 0; --x) {
        int colSum = cv::sum(Image.col(x))[0];
        // std::cout << "Column " << x << " sum: " << colSum << std::endl;
        if (colSum < thresholdValue * Image.rows) {
            right = x;
            break;
        }
    }
    // std::cout << "Right Coordinate: (" << right << ", " << top << ")" << std::endl;

    // Cropping
    if (top < bottom && left < right) {
        cv::Rect croppedRect(left, top, right - left + 1, bottom - top + 1);
        cv::Mat croppedImage = Original(croppedRect).clone();

        cv::imshow("Cropped Image", croppedImage);
        cv::waitKey(0);

        return croppedImage;
    } else {
        std::cerr << "Unable to find suitable area to crop." << std::endl;
        return Original;
    }
}
