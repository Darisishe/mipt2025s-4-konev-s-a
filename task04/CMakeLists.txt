set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DWORKING_DIRECTORY="${WORKING_DIRECTORY}")

add_executable(task04_01 task04_01.cpp)
target_link_libraries(task04_01 ${OpenCV_LIBS} semcv ${JSONCPP_LIBRARIES})

add_executable(task04_02 task04_02.cpp)
target_link_libraries(task04_02 ${OpenCV_LIBS} semcv)

add_executable(task04_03 task04_03.cpp)
target_link_libraries(task04_03 ${OpenCV_LIBS} semcv)
