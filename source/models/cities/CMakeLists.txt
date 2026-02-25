add_library(Cities STATIC)

target_sources(Cities PRIVATE source/cities.c)
target_compile_options(Cities PRIVATE -std=c99)
enable_sanitizers(Cities)

# Expose the public include directory of the Cities library
target_include_directories(Cities PUBLIC include)

# Link cJSON library, make sure any module including this module also links the cJSON library 
## /usr/lib/x86_64-linux-gnu/cmake/cJSON/cJSONConfig.cmake
find_package(cJSON 1.7.17 REQUIRED)
target_link_libraries(Cities PUBLIC ${CJSON_LIBRARY})
target_include_directories(Cities PRIVATE ${CJSON_INCLUDE_DIR})

# Build the unit test
add_executable(test_cities test/test_cities.c)

# Add debug symbols and disable optimizations for easier debugging
target_compile_options(test_cities PRIVATE -g -O0 -std=c99)

# Link the test with the library and Check
target_link_libraries(test_cities Cities)
target_include_directories(test_cities PRIVATE include)

# Find Check library and its dependencies for unit testing
find_library(CHECK_LIBRARY REQUIRED NAMES check)
find_path(CHECK_INCLUDE_DIR REQUIRED NAMES check.h)
find_library(M_LIBRARY REQUIRED NAMES m)
find_library(SUBUNIT_LIBRARY REQUIRED NAMES subunit)
find_library(PTHREAD_LIBRARY REQUIRED NAMES pthread)
target_link_libraries(test_cities ${CHECK_LIBRARY} ${SUBUNIT_LIBRARY} ${M_LIBRARY} ${PTHREAD_LIBRARY})
target_include_directories(test_cities PRIVATE ${CHECK_INCLUDE_DIR})
enable_sanitizers(test_cities)

# Add test to CTest with Check integration
enable_testing()
add_test(NAME cities_unit_tests COMMAND test_cities)

# Set environment for Check to output test results that CTest can parse
set_tests_properties(cities_unit_tests PROPERTIES
    ENVIRONMENT "CK_FORK=no"
)
