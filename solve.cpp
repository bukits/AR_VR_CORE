//
// Created by budai on 11/19/2023.
//
#include "rubik_cube.hpp"
#include "rubik_cube_solver.hpp"
#include <string>
#include <iostream>
#include "color_detector.hpp"

using namespace cv;
using namespace std;


std::string solve(const std::vector<cv::Mat> &faces) {
    int newWidth = 300;
    int newHeight = 300;

    Size newSize(newWidth, newHeight);
    Mat resizedImage;

    std::string cubeStateString;
    for (const auto &face: faces) {
        resize(face, resizedImage, newSize);
        cv::Mat colorMatrix = cv::Mat::zeros(3, 3, CV_8U);
        ColorDetector::StoreColors(resizedImage, colorMatrix);
        std::string faceColorString = ColorDetector::CreateCubeState(colorMatrix);

        cubeStateString.append(faceColorString);
    }


//solving the rubics cube
    const char *initial_state = ColorDetector::getStateFromDictionary(cubeStateString);

    rb::RubikCube rb(initial_state, 3);
    rb::RubikCubeSolver *solver = new rb::RubikCube3BasicSolver(rb);
    std::string moves = solver->Solve();
    /*std::istringstream iss(moves);
    std::string move;
    while (iss >> move) {
        rb.Move(move);
        rb.Dump();
    }*/

    return moves;
}

