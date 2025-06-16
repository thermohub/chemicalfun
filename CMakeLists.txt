# Require a certain version of cmake
cmake_minimum_required(VERSION 3.16)

# Set the name of the project
project(ChemicalFun VERSION 0.1.12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MACOSX_RPATH ON)

# Define variables with the GNU standard installation directories
include(GNUInstallDirs)

# Set the cmake module path of the project
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Use ccache to speed up repeated compilations
include(CCache)

# Ensure proper configuration if in a conda environment
include(CondaAware)

# Modify the TFUN_BUILD_* variables accordingly to BUILD_ALL
if(CHEMICALFUN_BUILD_ALL MATCHES ON)
    set(BUILD_SHARED_LIBS          ON)
    set(BUILD_STATIC_LIBS          ON)
    set(CHEMICALFUN_BUILD_TESTS    ON)
    set(CHEMICALFUN_BUILD_EXAMPLES ON)
    set(CHEMICALFUN_BUILD_PYTHON   ON)
endif()

if (CHEMICALFUN_WASM MATCHES ON)
    # Define which types of libraries to build
    option(BUILD_STATIC_LIBS "Build static libraries." ON)
    set(CHEMICALFUN_BUILD_PYTHON   ON)
else()
    # Ensure proper configuration if in a conda environment
    option(BUILD_SHARED_LIBS "Build shared libraries." ON)
    option(CHEMICALFUN_BUILD_EXAMPLES "Build examples." OFF)
    option(CHEMICALFUN_BUILD_TESTS "Build tests." ON)
    option(CHEMICALFUN_BUILD_PYTHON "Build the python wrappers and python package." ON)
    # Set libraries to be compiled with position independent code mode (i.e., fPIC option in GNU compilers)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Used into conda only
if(DEFINED ENV{CONDA_PREFIX})
   option(USE_SPDLOG_PRECOMPILED "Use spdlog in compiled version" ON)
else()
   option(USE_SPDLOG_PRECOMPILED "Use spdlog in compiled version" OFF)
endif()

# Set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    # The build type selection for the project
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type for ${PROJECT_NAME}." FORCE)

    # The build type options for the project
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
endif()

# Set the list of compiler flags for MSVC compiler
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_compile_options(
        /D_SCL_SECURE_NO_WARNINGS
        /D_CRT_SECURE_NO_WARNINGS=1
        /MP4
        /EHsc
        /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
        /DNOMINMAX
    )
endif()


if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-Wall -Wno-ignored-attributes -Wno-pedantic -Wno-variadic-macros -Wno-deprecated)
endif()

# test thread safe
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=thread")

# Find the Python interpreter
if(CHEMICALFUN_BUILD_PYTHON)
    find_package(pybind11 2.2.0 REQUIRED)
    message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
    if(NOT pybind11_FOUND)
        set(CHEMICALFUN_BUILD_PYTHON OFF)
        message(WARNING "pybind11 has not been found. "
            "Cannot build python bindings for chemicalfun")
    endif()
endif()

# Set the thermofun source directory path
set(CHEMICALFUN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Set the include directories
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    include_directories("/usr/local/include")
    link_directories("/usr/local/lib")
endif()
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    include_directories("c:/usr/local/include")
    link_directories("c:/usr/local/lib")
endif()
include_directories(${CHEMICALFUN_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all Reaktoro dependencies
include(ChemicalFunFindDeps)

# Build ChemicalFun library
add_subdirectory(src)

# Build the tests
if(CHEMICALFUN_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Build the examples
if(CHEMICALFUN_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build python wraper
if(CHEMICALFUN_BUILD_PYTHON)
    add_subdirectory(python)
endif()

# Install the cmake config files that permit users to use find_package(ChemicalFun)
include(ChemicalFunInstallCMakeConfigFiles)
