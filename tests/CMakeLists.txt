message(STATUS "Building test suite: ${CHEMICALFUN_BUILD_TESTS}")

include(CTest)
enable_testing()
find_package(Threads)
#set(INSTALL_GTEST OFF)
add_subdirectory(googletest REQUIRED)

add_executable(test_main  main.cpp)
target_link_libraries(test_main
    PUBLIC ChemicalFun
    PUBLIC gtest)
target_include_directories(test_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_test(NAME test_main COMMAND ./tests/test_main)


