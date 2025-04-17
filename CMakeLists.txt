cmake_minimum_required(VERSION 3.10)
project(verloc)

set(CMAKE_CXX_STANDARD 11)

# Add Loopers and util directories for header files.
include_directories(${CMAKE_SOURCE_DIR}/Loopers)
include_directories(${CMAKE_SOURCE_DIR}/util)

add_executable(verloc 
    main.cpp
    loopers/BaseLooper.cpp
    loopers/MasterLooper.cpp
    loopers/NetIntLooper.cpp
    util/sig_util.cpp
)
