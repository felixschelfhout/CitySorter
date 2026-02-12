cmake_minimum_required(VERSION 3.10)
project(CitySorter VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find required packages
find_package(CURL REQUIRED)
find_package(cJSON REQUIRED)

# Source files organized by module
set(CORE_SOURCES
    src/core/bst.c
)

set(CLI_SOURCES
    src/cli/main.c
)

set(CONNECTOR_SOURCES
    # Future: API connector implementations
)

set(MODEL_SOURCES
    # Future: Data model implementations
)

# All application sources
set(APP_SOURCES
    ${CLI_SOURCES}
    ${CORE_SOURCES}
    ${CONNECTOR_SOURCES}
    ${MODEL_SOURCES}
)

# Create executable
add_executable(citysorter ${APP_SOURCES})

# Link libraries
target_link_libraries(citysorter CURL::libcurl cjson)

# Set include directories for target
target_include_directories(citysorter PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Installation
install(TARGETS citysorter DESTINATION bin)

# Enable testing
enable_testing()

# Unit Tests
add_executable(test_bst tests/unit/test_bst.c src/core/bst.c)
target_include_directories(test_bst PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Add tests to CTest
add_test(NAME BSTUnitTests COMMAND test_bst)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
