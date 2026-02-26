add_library(CurlConnector STATIC)

target_sources(CurlConnector PRIVATE source/api.c)
target_compile_options(CurlConnector PRIVATE -std=c99)
enable_sanitizers(CurlConnector)

# Expose the public include directory at the connectors level (abstraction)
target_include_directories(CurlConnector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Link CURL library, make sure any module including this module also links the CURL library
## https://github.com/Kitware/CMake/blob/master/Modules/FindCURL.cmake 
find_package(CURL 8.5.0 REQUIRED COMPONENTS HTTPS SSL)
target_link_libraries(CurlConnector PUBLIC CURL::libcurl)

# Build the unit test
add_executable(test_api test/test_api.c)

# Add debug symbols and disable optimizations for easier debugging
target_compile_options(test_api PRIVATE -g -O0 -std=c99)

# Link the test with the library
target_link_libraries(test_api CurlConnector)

# Find Check library and its dependencies for unit testing
find_library(CHECK_LIBRARY REQUIRED NAMES check)
find_path(CHECK_INCLUDE_DIR REQUIRED NAMES check.h)
find_library(M_LIBRARY REQUIRED NAMES m)
find_library(SUBUNIT_LIBRARY REQUIRED NAMES subunit)
find_library(PTHREAD_LIBRARY REQUIRED NAMES pthread)
target_link_libraries(test_api ${CHECK_LIBRARY} ${SUBUNIT_LIBRARY} ${M_LIBRARY} ${PTHREAD_LIBRARY})
target_include_directories(test_api PRIVATE ${CHECK_INCLUDE_DIR})
enable_sanitizers(test_api)

# Add test to CTest with Check integration
enable_testing()
add_test(NAME api_connector_unit_tests COMMAND test_api)

# Set environment for Check to output test results that CTest can parse
set_tests_properties(api_connector_unit_tests PROPERTIES
    ENVIRONMENT "CK_FORK=no"
)

option(CITYSORTER_ENABLE_INTEGRATION_TESTS "Enable API integration tests" OFF)

if (CITYSORTER_ENABLE_INTEGRATION_TESTS)
    target_compile_definitions(test_api PRIVATE ENABLE_INTEGRATION_TESTS=1)
endif()
