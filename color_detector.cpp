#include <filesystem>
#include <iostream>
#include <map>
#include "color_detector.hpp"

std::string GetPath(){
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string path = cwd.generic_string();
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos != std::string::npos) {
        path = path.substr(0, lastSlashPos);
    }
    std::string folderPath = path + "/faces_test5";
    std::cout<< folderPath << std::endl;
    try {
        if (!std::filesystem::exists(folderPath)) {
            throw std::runtime_error("Folder not found: " + folderPath);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }

    return folderPath;
}

double measureBrightness(const cv::Mat& image) {
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::Scalar averageIntensity = mean(grayImage);
    return averageIntensity[0];
}

void adjustBrightness(cv::Mat& hsvImage, double targetBrightness = 40) {
    double currentBrightness = measureBrightness(hsvImage);

    if (currentBrightness < targetBrightness) {
        double brightnessFactor = targetBrightness / currentBrightness;

        hsvImage.forEach<cv::Vec3b>([brightnessFactor](cv::Vec3b& pixel, const int* position) {
            pixel[2] = cv::saturate_cast<uchar>(pixel[2] * brightnessFactor);
        });
    }
}

const char* ColorDetector::getStateFromDictionary(const std::string& input) {
    std::map<int, char> cubeStateDictionary = {
            /*{0, 'L'},
            {1, 'U'},
            {2, 'D'},
            {3, 'R'},
            {4, 'B'},
            {5, 'F'}*/
            // Based on the solver code
            {0, 'B'},
            {1, 'L'},
            {2, 'R'},
            {3, 'F'},
            {4, 'U'},
            {5, 'D'}
    };

    char* result = new char[input.size() + 1]; // +1 for null-terminator
    int resultIndex = 0;

    for (char c : input) {
        int key = c - '0';
        auto it = cubeStateDictionary.find(key);

        if (it != cubeStateDictionary.end()) {
            result[resultIndex++] = it->second;
        }
    }

    result[resultIndex] = '\0'; // Null-terminate the result string

    return result;
}

std::vector<cv::Mat> ColorDetector::LoadImages() {
    std::string folderName = GetPath();

    std::vector<cv::Mat> images;
    // cv::String folderName = folderPath + "*.png";
    std::vector<cv::String> fileNames;
    if (std::filesystem::exists(folderName)) {
        cv::glob(folderName, fileNames, false);

        if (fileNames.empty()) {
            std::cout << "No files found in the folder." << std::endl;
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

    /*for (const auto& image : images) {
        imshow("Loaded Image", image);
        cv::waitKey(0);
    }*/


    return images;
}

void ColorDetector::StoreColors(cv::Mat& image, cv::Mat& colorMatrix) {
    int numRows = colorMatrix.rows;
    int numCols = colorMatrix.cols;
    int cellWidth = image.cols / numCols;
    int cellHeight = image.rows / numRows;

    std::vector<int> colorIds = {0, 1, 2, 3, 4, 5}; //R, G, B, O, Y, W

    std::vector<cv::Scalar> lowerBounds = {
            cv::Scalar(151, 50, 50),  // Red
            cv::Scalar(50, 50, 40),  // Green
            cv::Scalar(101, 150, 50), // Blue
            cv::Scalar(0, 50 ,100),   // Orange
            cv::Scalar(16, 50, 70), // Yellow
            cv::Scalar(0, 0, 90) // White
    };

    std::vector<cv::Scalar> upperBounds = {
            cv::Scalar(180, 255, 255),  // Red
            cv::Scalar(90, 255, 255),  // Green
            cv::Scalar(140, 255, 255), // Blue
            cv::Scalar(15, 255, 255),   // Orange
            cv::Scalar(45, 255, 255), // Yellow
            cv::Scalar(180, 90, 255) // White
    };
    cv::Mat areaMatrix = cv::Mat::zeros(3, 3, CV_64F);

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    /*
    adjustBrightness(hsvImage, 200);
    cvtColor(hsvImage, hsvImage, cv::COLOR_HSV2BGR);


    cv::imshow("Result", hsvImage);
    cv::waitKey(0);
    cv::destroyAllWindows()*/

    for (int i = 0; i < colorIds.size(); ++i) {
        cv::Mat mask;
        cv::Scalar lowerBound = lowerBounds[i];
        cv::Scalar upperBound = upperBounds[i];
        cv::inRange(hsvImage, lowerBound, upperBound, mask);


        /*
        cv::imshow("Result", mask);
        cv::waitKey(0);
        cv::destroyAllWindows();*/

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

        cv::Mat colorMasked;
        cv::bitwise_and(image, image, colorMasked, mask);

        std::vector<std::vector<cv::Point>> contours;
        findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int colorId = colorIds[i];

        for (const auto & contour : contours) {
            double area = cv::contourArea(contour);

            cv::Moments mu = moments(contour);
            cv::Point2f centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);

            int cellRow = static_cast<int>(centroid.y / cellHeight);
            int cellCol = static_cast<int>(centroid.x / cellWidth);

            if (cellRow >= 0 && cellRow < numRows && cellCol >= 0 && cellCol < numCols) {

                if (area > areaMatrix.at<double>(cellRow, cellCol)) {
                    areaMatrix.at<double>(cellRow, cellCol) = area;
                    colorMatrix.at<uchar>(cellRow, cellCol) = colorId;
                }
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
