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
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main() {

    // color detection
    Mat face = imread("C:\\Users\\tbukits\\CLionProjects\\AR_VR_CORE\\face.png");

    Mat hsvImage;
    cvtColor(face, hsvImage, COLOR_BGR2HSV);

    Scalar lowerBound(0, 100, 100);
    Scalar upperBound(10, 255, 255);

    Mat mask;
    inRange(hsvImage, lowerBound, upperBound, mask);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(mask, mask, MORPH_OPEN, kernel);
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);

    Mat redMasked;
    bitwise_and(face, face, redMasked, mask);

    // Display the results
    imshow("Original Image", face);
    imshow("Red Color Mask", redMasked);
    waitKey(0);


    /*
    //solving the rubics cube
    const char* initial_state = "FFFFFFFFFUUUUUUUUUDDDDDDDDDLLLLLLLLLRRRRRRRRRBBBBBBBBB";

    rb::RubikCube rb(initial_state, 3);


    std::cout << "Scramble cube:" << std::endl;
    std::string moves = rb.Scramble();
    rb.Dump();

    std::cout << "Inverse scramble steps:" << std::endl;
    rb.Inverse(moves);
    rb.Dump();

    std::cout << "Scramble cube again:" << std::endl;
    rb.Scramble();
    rb.Dump();

    rb::RubikCubeSolver *solver = new rb::RubikCube3BasicSolver(rb);
    string moves = solver->Solve();

    std::cout << "Moves solved by basic solver: " << moves << std::endl;
    rb.Move(moves);
    rb.Dump();
     */

    return 0;
}