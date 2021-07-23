cmake_minimum_required(VERSION 3.1)
project(opencv_example)

find_package(OpenCV 4 REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})

# Cara Build
# pakai "cmake ." dulu
# lalu pake "make"
# 
# Run pakai ./"nama file"
# ex = ./Main