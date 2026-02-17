cmake_minimum_required(VERSION 3.28)

project(
    citysorter
    VERSION 0.1.0
    DESCRIPTION "Discover all cities of the countries of your dreams - neatly sorted in a binary search tree"
    HOMEPAGE_URL "https://github.com/felixschelfhout/CitySorter"
    LANGUAGES C
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(cmake/Tooling.cmake)

# Generate ${PROJECT_BINARY_DIR}/include/version.h from version.h.in
configure_file(source/cli/version.h.in include/version.h)
list(APPEND EXTRA_INCLUDES ${PROJECT_BINARY_DIR}/include)

# Configure the Binary Search Tree library
add_subdirectory(source/core/bst)
list(APPEND EXTRA_LIBS BinarySearchTree)

# Create Application
add_executable(citysorter)

# Set Application sources
target_sources(citysorter PRIVATE source/cli/main.c)

# Set Application required libraries
## https://github.com/Kitware/CMake/blob/master/Modules/FindCURL.cmake 
find_package(CURL 8.5.0 REQUIRED COMPONENTS HTTPS SSL) 
target_link_libraries(citysorter CURL::libcurl)
## /usr/lib/x86_64-linux-gnu/cmake/cJSON/cJSONConfig.cmake
find_package(cJSON 1.7.17 REQUIRED) 
target_link_libraries(citysorter ${CJSON_LIBRARY})
## Local libraries
target_link_libraries(citysorter ${EXTRA_LIBS})

# Set Application compiler flags
target_compile_features(citysorter PRIVATE c_std_99)
target_compile_options(citysorter PRIVATE -Wall -Wextra -Wpedantic)
enable_sanitizers(citysorter)

# Set Application include directories
target_include_directories(citysorter PRIVATE ${PROJECT_SOURCE_DIR}/include)
## Local include directories
target_include_directories(citysorter PRIVATE ${EXTRA_INCLUDES})

# Test the Application
enable_testing()
add_test(NAME citysorter_runs COMMAND citysorter)
add_test(NAME citysorter_version COMMAND citysorter --version)
set_tests_properties(citysorter_version PROPERTIES PASS_REGULAR_EXPRESSION "citysorter ${PROJECT_VERSION}")
add_test(NAME citysorter_help COMMAND citysorter --help)
set_tests_properties(citysorter_help PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*OPTIONS")
add_test(NAME citysorter_unknown_command COMMAND citysorter --unknown)
set_tests_properties(citysorter_unknown_command PROPERTIES WILL_FAIL TRUE)

