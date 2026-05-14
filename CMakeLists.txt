cmake_minimum_required(VERSION 4.1)
project(nBody)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(nBody main.cpp
        sim.h
        body.h
        constants.h)

target_link_libraries(nBody PRIVATE SFML::Graphics SFML::Window SFML::System)