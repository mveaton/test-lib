cmake_minimum_required(VERSION 3.23)

# Project Statement
project(
        system_API
        VERSION 1.0
        LANGUAGES CXX C)

message("System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

# ---------------------------------------------------------------------------------------
# Set library configuration
# ---------------------------------------------------------------------------------------

# Determine system processor to support correct path for libraries
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set(OPENCV_DIR   opencv_x86_64)
    set(arch x86_64)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    set(OPENCV_DIR   opencv_aarch64)
    set(arch aarch64)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
    set(OPENCV_DIR   opencv_arm64)
    set(arch arm64)
else()
    message( FATAL_ERROR "Unknown System Processor, CMake will exit." )
endif()

# Find packages used in the system library or its linked in libraries
find_package(OpenEXR REQUIRED)
find_package(Eigen3 3.0.0)
find_package (Python3 COMPONENTS Interpreter Development)

# Find the local OpenCV package
set(OpenCV_PATH  ${CMAKE_CURRENT_LIST_DIR}/vendor/install/${OPENCV_DIR}/lib/cmake/opencv4)
message("OpenCV Path: ${OpenCV_PATH}")
find_package(OpenCV 4.6 EXACT REQUIRED PATHS "${OpenCV_PATH}" NO_DEFAULT_PATH)
message("OpenCV version: ${OpenCV_VERSION}")


# Add headers directories for the project
include_directories(
        src
        ${OpenCV_INCLUDE_DIRS})

# ---------------------------------------------------------------------------------------
# Set compile options
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)

# ---------------------------------------------------------------------------------------
# Setup executable
# ---------------------------------------------------------------------------------------
# Create executable
add_executable(system_API
               src/main.cc
               src/TactNib/target_image/target_image.cc
               src/TactNib/target_image/target_image.h
        src/TactNib/target_image/target_finder_strategy.cc
        src/TactNib/target_image/target_finder_strategy.h
        src/TactNib/target_image/object_detect_strategy.cc
        src/TactNib/target_image/object_detect_strategy.h
        src/TactNib/target_image/opencv_strategy.cc
        src/TactNib/target_image/opencv_strategy.h
        src/TactNib/target_image/opencv_algo.cc
        src/TactNib/target_image/opencv_algo.h src/TactNib/target_image/support.h)

# link libraries
target_link_libraries(system_API ${OpenCV_LIBS})