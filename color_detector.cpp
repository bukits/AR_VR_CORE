
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "color_detector.hpp"
#include "Face.hpp"
#include <filesystem>

std::string GetPath(){
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string path = cwd.generic_string();
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos != std::string::npos) {
        path = path.substr(0, lastSlashPos);
    }
    std::string folderPath = path + "/faces/";
    // std::cout<< folderPath << std::endl;

    return folderPath;
}

cv::Mat ColorDetector::LoadImages() {
    std::string folderName = GetPath();

    std::vector<cv::Mat> images;
    // cv::String folderName = folderPath + "*.png";
    std::vector<cv::String> fileNames;
    if (std::filesystem::exists(folderName)) {
        cv::glob(folderName, fileNames, false);

        if (fileNames.empty()) {
            std::cerr << "No files found in the folder." << std::endl;
        }
        else {
            for (const cv::String& fileName : fileNames) {
                cv::Mat image = cv::imread(fileName);
                if (!image.empty()) {
                    images.push_back(image);
                }
            }
        }
    }
    else {
        throw std::runtime_error("Folder not found: " + folderName);
    }
    /*
    for (const auto& image : images) {
        imshow("Loaded Image", image);
        cv::waitKey(0);
    }
    */

    return  images[0];
}

cv::Mat ColorDetector::FaceDetection(cv::Mat &image) {
    cv::Mat img = image;
    new Face();
    cv::Mat modified = Face::Thresholding(img);
    Face::Contour(modified);

    return img;
}




void ColorDetector::StoreColors(cv::Mat& image, cv::Mat& colorMatrix) {
    int numRows = colorMatrix.rows;
    int numCols = colorMatrix.cols;
    int cellWidth = image.cols / numCols;
    int cellHeight = image.rows / numRows;

    std::vector<int> colorIds = {0, 1, 2, 3, 4, 5}; //R, G, B, O, Y, W

    std::vector<cv::Scalar> lowerBounds = {
            cv::Scalar(0, 100, 100),  // Red
            cv::Scalar(35, 100, 100),  // Green
            cv::Scalar(100, 100, 100), // Blue
            cv::Scalar(10, 100, 100),   // Orange
            cv::Scalar(20, 100, 100), // Yellow
            cv::Scalar(0, 0, 200) // White
    };

    std::vector<cv::Scalar> upperBounds = {
            cv::Scalar(10, 255, 255),  // Red
            cv::Scalar(85, 255, 255),  // Green
            cv::Scalar(130, 255, 255), // Blue
            cv::Scalar(30, 255, 255),   // Orange
            cv::Scalar(30, 255, 255), // Yellow
            cv::Scalar(1800, 30, 255) // White
    };

    for (int i = 0; i < colorIds.size(); ++i) {
        cv::Mat hsvImage;
        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        cv::Scalar lowerBound = lowerBounds[i];
        cv::Scalar upperBound = upperBounds[i];
        cv::inRange(hsvImage, lowerBound, upperBound, mask);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

        cv::Mat colorMasked;
        cv::bitwise_and(image, image, colorMasked, mask);

        std::vector<std::vector<cv::Point>> contours;
        findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int colorId = colorIds[i];

        for (const auto & contour : contours) {
            cv::Moments mu = moments(contour);
            cv::Point2f centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);

            int cellRow = static_cast<int>(centroid.y / cellHeight);
            int cellCol = static_cast<int>(centroid.x / cellWidth);

            if (cellRow >= 0 && cellRow < numRows && cellCol >= 0 && cellCol < numCols) {
                colorMatrix.at<uchar>(cellRow, cellCol) = colorId ;
            }
        }
    }
}

std::string ColorDetector::CreateCubeState(cv::Mat &colorMatrix) {
    char* face = new char[9];
    int charIndex = 0;

    for (int row = 0; row < colorMatrix.rows; row++) {
        for (int col = 0; col < colorMatrix.cols; col++) {
            char colorChar = static_cast<char>('0' + colorMatrix.at<uchar>(row, col));
            face[charIndex] = colorChar;
            charIndex++;
        }
    }
    std::string faceString(face, face + 9);
    delete[] face;
    return faceString;
}


