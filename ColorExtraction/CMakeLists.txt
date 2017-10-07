cmake_minimum_required (VERSION 2.6)

project (ColorExtraction)

add_definitions(-std=gnu++11)

file (GLOB COLOR_EXTRACTION_CORE ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories(
)

set (SOURCE "${COLOR_EXTRACTION_CORE};")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g3")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g3")


add_executable(ColorExtraction ${SOURCE})

target_link_libraries(ColorExtraction
	opencv_core
	opencv_highgui 
	opencv_objdetect
	opencv_imgproc
	boost_system
	boost_filesystem
)
