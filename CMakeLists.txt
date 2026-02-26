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

# Configure the Cities model library
add_subdirectory(source/models/cities)
list(APPEND EXTRA_LIBS Cities)

# Configure the Curl Connector library
add_subdirectory(source/connectors/curl)
list(APPEND EXTRA_LIBS CurlConnector)

# Create Application
add_executable(citysorter)

# Set Application sources
target_sources(citysorter PRIVATE source/cli/main.c)

# Set Application required libraries
## Local libraries
target_link_libraries(citysorter ${EXTRA_LIBS})

# Set Application compiler flags
target_compile_features(citysorter PRIVATE c_std_99)
target_compile_options(citysorter PRIVATE -std=c99 -Wall -Wextra -Wpedantic)
enable_sanitizers(citysorter)

# Set Application include directories
target_include_directories(citysorter PRIVATE ${PROJECT_SOURCE_DIR}/include)
## Local include directories
target_include_directories(citysorter PRIVATE ${EXTRA_INCLUDES})

# Test the Application
include(tests/e2e/CliTest.cmake)

