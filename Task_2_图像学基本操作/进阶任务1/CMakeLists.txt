cmake_minimum_required(VERSION 2.8)
project(segmentation)
set(CMAKE_CXX_STANDARD 11)

# 查找OpenCV库
find_package(OpenCV REQUIRED)

# 添加OpenCV头文件的路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 定义一个变量 SOURCE_FILES，并将它设置为包含两个源文件的列表，这两个源文件是 main.cpp 和 segment.cpp。便于维护
set(SOURCE_FILES main.cpp)

# 创建可执行文件
add_executable(gamma ${SOURCE_FILES})

# 链接OpenCV库
target_link_libraries(gamma ${OpenCV_LIBS})

