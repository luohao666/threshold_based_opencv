cmake_minimum_required(VERSION 2.8)
project( threshold_demo )

#set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread -lstdc++ -lm")

file(GLOB SOURCES "*.cpp")
find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( main ${SOURCES} )
target_link_libraries( main ${OpenCV_LIBS} )
