# Path to dir, where samples/ located
set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DWORKING_DIRECTORY="${WORKING_DIRECTORY}")

add_executable(generator_task01 samples_generator.cpp)
target_link_libraries(generator_task01 ${OpenCV_LIBS} semcv)

add_executable(task01 task01.cpp)
target_link_libraries(task01 ${OpenCV_LIBS} semcv)
