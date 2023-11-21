cmake_minimum_required(VERSION 3.26)
project(AR_VR_CORE)

set(CMAKE_CXX_STANDARD 17)
set(ENV{OPENCV_DIR} "C:\\opencv\\build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#add_library(AR_VR_CORE SHARED
#        rubik_cube.cpp
#        rubik_cube_3basic_solver.cpp
#        color_detector.cpp
#        color_detector.cpp)

add_executable(AR_VR_CORE main.cpp
        rubik_cube.cpp
        rubik_cube_3basic_solver.cpp
        color_detector.cpp
        color_detector.hpp
        Solver.cpp
        Solver.hpp

)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(AR_VR_CORE ${OpenCV_LIBS})