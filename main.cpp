/*`
 *   Copyright 2017 Toby Liu
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#include "rubik_cube.hpp"
#include "rubik_cube_solver.hpp"
#include <string>
#include <iostream>
#include "color_detector.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat face = imread(R"(C:\Users\tbukits\CLionProjects\AR_VR_CORE\faces\1top.png)");

    int newWidth = 300;
    int newHeight = 300;

    Size newSize(newWidth, newHeight);
    Mat resizedImage;
    resize(face, resizedImage, newSize);

    cv::Mat colorMatrix = cv::Mat::zeros(3, 3, CV_8U);

    ColorDetector::StoreColors(resizedImage, colorMatrix);
    std::string  faceColorString = ColorDetector::CreateCubeState(colorMatrix);

    std::cout << colorMatrix << endl;
    cout << faceColorString << endl;


    //solving the rubics cube
    /*
    const char* initial_state = "FFFFFFFFFUUUUUUUUUDDDDDDDDDLLLLLLLLLRRRRRRRRRBBBBBBBBB";


    rb::RubikCube rb(3);

    std::cout << "Scramble cube:" << std::endl;
    std::string moves = rb.Scramble();
    cout << moves << endl;
    rb.Dump();
    cout << rb.faces_ << endl;

    rb::RubikCubeSolver *solver = new rb::RubikCube3BasicSolver(rb);
    moves = solver->Solve();

    std::cout << "Moves solved by basic solver: " << moves << std::endl;
    rb.Move(moves);
    rb.Dump();*/

    return 0;
}