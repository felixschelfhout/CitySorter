add_library(BinarySearchTree STATIC)

target_sources(BinarySearchTree PRIVATE source/bst.c)

# Expose the public include directory of the Binary Search Tree library
# Also used by the library itself, so it is not added as an "INTERFACE" include directory, 
# but as a "PUBLIC" include directory
target_include_directories(BinarySearchTree PUBLIC include)

# Build the unit test
add_executable(test_bst test/test_bst.c)

# Link the test with the library and Check
target_link_libraries(test_bst BinarySearchTree)
find_library(CHECK_LIBRARY NAMES check libcheck REQUIRED)
find_library(SUBUNIT_LIBRARY NAMES subunit libsubunit)
find_library(M_LIBRARY NAMES m)
find_library(RT_LIBRARY NAMES rt)
find_path(CHECK_INCLUDE_DIR check.h REQUIRED)
target_link_libraries(test_bst ${CHECK_LIBRARY})
if(SUBUNIT_LIBRARY)
    target_link_libraries(test_bst ${SUBUNIT_LIBRARY})
endif()
if(M_LIBRARY)
    target_link_libraries(test_bst ${M_LIBRARY})
endif()
if(RT_LIBRARY)
    target_link_libraries(test_bst ${RT_LIBRARY})
endif()
target_link_libraries(test_bst pthread)
target_include_directories(test_bst PRIVATE ${CHECK_INCLUDE_DIR})

# Add test to CTest with Check integration
enable_testing()
add_test(NAME bst_unit_tests COMMAND test_bst)

# Set environment for Check to output test results that CTest can parse
set_tests_properties(bst_unit_tests PROPERTIES
    ENVIRONMENT "CK_FORK=no"
)
