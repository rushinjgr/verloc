cmake_minimum_required(VERSION 3.10)
project(Loopers)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/Loopers)

add_executable(loopers 
    main.cpp
    loopers/BaseLooper.cpp
    loopers/MasterLooper.cpp
    loopers/NetIntLooper.cpp
)
