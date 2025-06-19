cmake_minimum_required(VERSION 3.10)
project(EMFieldVisualizer)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 and related modules
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    include
)

# Create executable
add_executable(EMFieldVisualizer
    src/main.cpp
    src/Charge.cpp
    src/FieldCalculator.cpp
    src/GUI.cpp
)

# Explicitly link all required libraries
target_link_libraries(EMFieldVisualizer
    ${SDL2_LIBRARIES}
    SDL2_image
    SDL2_ttf
)
