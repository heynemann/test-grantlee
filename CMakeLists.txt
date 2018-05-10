# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.9.4)

# Project's Name
project(api)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Grantlee5 REQUIRED)
find_package(Qt5 5.6.0 REQUIRED COMPONENTS Core Network Widgets)
find_package(Qt5Widgets REQUIRED) 
find_package(Cutelyst2Qt5 2.0.0 REQUIRED)
find_package(Boost 1.52 COMPONENTS system thread REQUIRED)

 
# Project's version number
set(API_VERSION_MAJOR 0)
set(API_VERSION_MINOR 1)
set(API_VERSION_REVISION 0)

# Configure a header file to include
# configurations in our program
configure_file(
    "${PROJECT_SOURCE_DIR}/src/apiConfig.h.in"
    "${PROJECT_BINARY_DIR}/src/apiConfig.h"
)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# The following folder will be included
include_directories(
    ${PROJECT_BINARY_DIR}   
    ${Cutelyst2Qt5_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)
 
# Using C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5Widgets)

# Now we create an executable
add_executable(api ./src/main.cpp)

# Now a lib
#add_library(MyApp SHARED ./src/hello.cpp ./src/root.cpp)

target_link_libraries(
    api
	pthread
    Grantlee5::Templates
    Cutelyst::Core
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
	${Boost_LIBRARIES}
)

#target_link_libraries(MyApp
	#Cutelyst::Core
    #Cutelyst::StaticSimple
    #Cutelyst::View::Grantlee
    #Qt5::Core
	#Qt5::Network
#)
