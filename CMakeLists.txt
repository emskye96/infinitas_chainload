cmake_minimum_required(VERSION 3.17)
project(infinitas_chainload)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(UNICODE)
add_library(${PROJECT_NAME} SHARED src/library.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "d3d9")