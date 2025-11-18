message(STATUS "Building test suite: ${CHEMICALFUN_BUILD_TESTS}")

include(CTest)
find_package(Threads)
#set(INSTALL_GTEST OFF)
add_subdirectory(googletest REQUIRED)

add_executable(test_main  main.cpp)
target_link_libraries(test_main
    PUBLIC ChemicalFun
    PUBLIC gtest)
#target_include_directories(test_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

enable_testing()
add_test(NAME test_main COMMAND test_main)


# https://google.github.io/googletest/quickstart-cmake.html
#
#include(GoogleTest)
#gtest_add_tests(TARGET test_main
#        TEST_SUFFIX .noArgs
#        TEST_LIST   noArgsTests
#    )
#set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)
