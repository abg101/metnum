cmake_minimum_required(VERSION 3.8)
project(ppmloader)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        ppmloader.cpp
        ppmloader.h)

add_executable(ppmloader ${SOURCE_FILES})
