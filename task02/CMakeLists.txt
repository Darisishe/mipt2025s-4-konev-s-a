set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DWORKING_DIRECTORY="${WORKING_DIRECTORY}")

add_executable(task02 task02.cpp)
target_link_libraries(task02 ${OpenCV_LIBS} semcv)

add_executable(task02_distr_params distribution_params.cpp)
target_link_libraries(task02_distr_params ${OpenCV_LIBS} semcv)
