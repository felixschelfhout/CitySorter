cmake_minimum_required(VERSION 3.10)
project(CitySorter VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Find required packages
find_package(CURL REQUIRED)
find_package(cJSON REQUIRED)

# Add subdirectories
# TODO

# Create executable
add_executable(citysorter main.c)


# Link libraries
target_link_libraries(citysorter CURL::libcurl cjson)

# Installation
install(TARGETS citysorter DESTINATION bin)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
