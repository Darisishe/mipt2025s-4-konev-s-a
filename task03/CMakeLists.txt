set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DWORKING_DIRECTORY="${WORKING_DIRECTORY}")

add_executable(task03_test_contrast test_contrast.cpp)
target_link_libraries(task03_test_contrast ${OpenCV_LIBS} semcv)

add_executable(task03 task03.cpp)
target_link_libraries(task03 ${OpenCV_LIBS} semcv)
