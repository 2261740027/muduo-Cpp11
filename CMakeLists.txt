cmake_minimum_required(VERSION 3.5)
project(muduo)

add_definitions("-Wall -g")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#添加子目录
add_subdirectory(Src)  
#添加头文件
include_directories(Inc)

#生成可执行文件
add_executable(${PROJECT_NAME} main.cpp)
#链接静态库
target_link_libraries(${PROJECT_NAME} eventLoopLib)   
