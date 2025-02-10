add_library(semcv semcv.cpp)
target_include_directories(semcv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(semcv PRIVATE ${OpenCV_LIBS})
