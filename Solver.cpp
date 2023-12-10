#include "Solver.hpp"
#include "rubik_cube.hpp"
#include "rubik_cube_solver.hpp"
#include <string>
#include <iostream>
#include <map>
#include <future>
#include <chrono>

using namespace cv;
using namespace std;


std::string Solver::solve(const std::vector<cv::Mat> &faces) {
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

// Check color counts
    std::map<char, int> colorCounts;
    for (char c : cubeStateString) {
        colorCounts[c]++;
    }

    for (const auto &pair : colorCounts) {
        if (pair.second != 9) {
            cerr<< "Warning: Color detection might be wrong. Not all colors appear 9 times." << std::endl;
            return "";
        }
    }


//solving the rubics cube
    const char *initial = ColorDetector::getStateFromDictionary(cubeStateString);
    rb::RubikCube rb(initial, 3);

    Solver::cube = rb;
    std::string temp_string = rb.GetCubeString();
    Solver::initial_state = temp_string;
    std::vector<std::string> t;
    for (size_t i = 0; i < 6; i++) {
        t.push_back(temp_string.substr(i * 9, 9));
    }
    Solver::all_states.push_back(t);

    auto future = std::async(std::launch::async, [&]() {
        rb::RubikCubeSolver *solver = new rb::RubikCube3BasicSolver(rb);
        return solver->Solve();
    });
    // Wait for the result with a timeout
    std::chrono::seconds timeout(5); // 5 seconds timeout
    if (future.wait_for(timeout) == std::future_status::timeout) {
        // Solver did not finish in time, return and print error message
        std::cerr << "Error: Solver timed out." << std::endl;
        return "";
    } else {
        // Solver finished, get the result
        std::string moves = future.get();
        std::istringstream iss(moves);
        std::string move;
        while (iss >> move) {
            rb.Move(move);
            string s = rb.GetCubeString();
            vector<string> temp;
            for (size_t i = 0; i < 6; i++) {
                temp.push_back(s.substr(i * 9, 9));
            }
            Solver::all_states.push_back(temp);
            Solver::steps.push_back(move);
        }
        return moves;
    }
}

bool Solver::check_state(const cv::Mat &face) {

    int newWidth = 300;
    int newHeight = 300;

    Size newSize(newWidth, newHeight);
    Mat resizedImage;

    resize(face, resizedImage, newSize);
    cv::Mat colorMatrix = cv::Mat::zeros(3, 3, CV_8U);
    ColorDetector::StoreColors(resizedImage, colorMatrix);
    std::string faceColorString = ColorDetector::CreateCubeState(colorMatrix);
    std::string checked_state = ColorDetector::getStateFromDictionary(faceColorString);

    for (const auto &states: Solver::all_states) {
        for (const auto &state: states) {
            if (state == checked_state) {
                return true;
            }
        }

    }
    return false;
}