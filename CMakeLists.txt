cmake_minimum_required(VERSION 3.22)
project(opengl_rubik)

set(CMAKE_CXX_STANDARD 14)

add_executable(opengl_rubik main.cpp)
target_link_libraries(opengl_rubik -lGL -lGLU -lglut)
