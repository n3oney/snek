cmake_minimum_required(VERSION 3.17)
project(firstGraphics)

set(CMAKE_CXX_STANDARD 20)

add_executable(snek main.cpp)
SET(CMAKE_CXX_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system -s")

add_custom_command(TARGET snek POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "../PressStart2P-Regular.ttf"
        "./"
        COMMENT "Copying to output directory")