add_library(
        semcv
        semcv.hpp 
        semcv_task01.cpp
        semcv_task02.cpp
        semcv_task03.cpp
        semcv_task04.cpp
        semcv_task05.cpp
        semcv_task06.cpp
        )

set_property(TARGET semcv PROPERTY CXX_STANDARD 20)

target_include_directories(semcv PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)


target_link_libraries(semcv ${JSONCPP_LIBRARIES} ${OpenCV_LIBS})