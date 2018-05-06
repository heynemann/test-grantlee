# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.9.4)

# Project's Name
project(api)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Grantlee5 REQUIRED)

# Project's version number
set(API_VERSION_MAJOR 0)
set(API_VERSION_MINOR 1)
set(API_VERSION_REVISION 0)

# Using C++ 17
set(CMAKE_CXX_STANDARD 17)

# Configure a header file to include
# configurations in our program
configure_file(
    "${PROJECT_SOURCE_DIR}/src/apiConfig.h.in"
    "${PROJECT_BINARY_DIR}/src/apiConfig.h"
)

# The following folder will be included
include_directories("${PROJECT_BINARY_DIR}")
 
# Using C++ 17
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets)

# Now we create an executable
add_executable(api ./src/main.cpp)

target_link_libraries(
    api
    Qt5::Widgets
    Grantlee5::Templates
)
