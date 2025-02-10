cmake_minimum_required(VERSION 3.10)
project(mipt2025s_konev_s_a)

find_package(OpenCV REQUIRED)

add_subdirectory(semcv)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE semcv)
