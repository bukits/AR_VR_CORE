#ifndef AR_VR_CORE_SOLVER_HPP
#define AR_VR_CORE_SOLVER_HPP


#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "rubik_cube.hpp"
#include "rubik_cube_solver.hpp"
#include <string>
#include <iostream>
#include "color_detector.hpp"

class Solver {
public:
    Solver() = default;
    ~Solver() = default;

    std::string solve(const std::vector<cv::Mat> &faces);
    bool check_state(const cv::Mat &face);

private:
    rb::RubikCube cube;                                 // For representing the cube
    std::string initial_state;                          // For storing the initial state of the cube
    std::vector<std::string> steps;                     // For storing the moves made on the cube
    std::vector<std::vector<std::string>> all_states;   // For storing all the states during the solve algorithm
};


#endif //AR_VR_CORE_SOLVER_HPP
