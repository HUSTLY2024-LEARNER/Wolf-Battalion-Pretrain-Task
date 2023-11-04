cmake_minimum_required(VERSION 2.8)
project(distor)
set(CMAKE_CXX_STANDARD 11)



find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

##include_directories( "/usr/include/eigen3" )

set(SOURCE_FILES main.cpp)

add_executable(distor ${SOURCE_FILES})

target_link_libraries(distor ${OpenCV_LIBS})