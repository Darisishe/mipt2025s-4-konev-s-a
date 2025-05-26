add_library(
        semcv
        semcv.hpp 
        semcv_task01.cpp
        )
        
set_property(TARGET semcv PROPERTY CXX_STANDARD 20)
target_include_directories(semcv PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(semcv ${OpenCV_LIBS})