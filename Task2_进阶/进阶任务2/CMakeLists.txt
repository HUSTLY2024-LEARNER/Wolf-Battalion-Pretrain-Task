cmake_minimum_required(VERSION 2.8)
project(affine)
set(CMAKE_CXX_STANDARD 11)

# 查找OpenCV库
find_package(OpenCV REQUIRED)

# 添加OpenCV头文件的路径
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)

# 创建可执行文件
add_executable(affine ${SOURCE_FILES})

# 链接OpenCV库
target_link_libraries(affine ${OpenCV_LIBS})

