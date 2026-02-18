add_library(BinarySearchTree STATIC)

target_sources(BinarySearchTree PRIVATE source/bst.c)
enable_sanitizers(BinarySearchTree)

# Expose the public include directory of the Binary Search Tree library
# Also used by the library itself, so it is not added as an "INTERFACE" include directory, 
# but as a "PUBLIC" include directory
target_include_directories(BinarySearchTree PUBLIC include)

# Build the unit test
add_executable(test_bst test/test_bst.c)

# Add debug symbols and disable optimizations for easier debugging
target_compile_options(test_bst PRIVATE -g -O0)

# Link the test with the library and Check
target_link_libraries(test_bst BinarySearchTree)
target_include_directories(test_bst PRIVATE include)

# Find Check library and its dependencies for unit testing
find_library(CHECK_LIBRARY REQUIRED NAMES check)
find_path(CHECK_INCLUDE_DIR REQUIRED NAMES check.h)
find_library(M_LIBRARY REQUIRED NAMES m)
find_library(SUBUNIT_LIBRARY REQUIRED NAMES subunit)
find_library(PTHREAD_LIBRARY REQUIRED NAMES pthread)
target_link_libraries(test_bst ${CHECK_LIBRARY} ${SUBUNIT_LIBRARY} ${M_LIBRARY} ${PTHREAD_LIBRARY})
target_include_directories(test_bst PRIVATE ${CHECK_INCLUDE_DIR})
enable_sanitizers(test_bst)

# Add test to CTest with Check integration
enable_testing()
add_test(NAME bst_unit_tests COMMAND test_bst)

# Set environment for Check to output test results that CTest can parse
set_tests_properties(bst_unit_tests PROPERTIES
    ENVIRONMENT "CK_FORK=no"
)
