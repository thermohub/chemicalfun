# Collect the source files of PyChemicalData
file(GLOB_RECURSE HEADER_FILES *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

# Include the ThermoFun/python directory
include_directories(${PROJECT_SOURCE_DIR}/python)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/installed/include/eigen3")
	# be friendly to our fellow developers, do not pollute the userspace with dev stuff
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/extern/installed/include/eigen3")
else()
    include_directories(${CONDA_AWARE_PREFIX}/include/eigen3)
    include_directories("/usr/local/include/eigen3")
endif()


# Create the PyChemicalData library
pybind11_add_module(PyChemicalFun ${SOURCE_FILES} NO_EXTRAS) # NO_EXTRAS used to decrease linking time, but causes larger module file size

# Link PyChemicalData against ChemicalData library
target_link_libraries(PyChemicalFun PUBLIC ChemicalFun::ChemicalFun)

# install(TARGETS PyChemicalFun DESTINATION site-packages)

# https://github.com/pybind/cmake_example/blob/master/.pre-commit-config.yaml

# https://pybind11.readthedocs.io/en/stable/cmake/index.html


#! https://realpython.com/python-pyproject-toml/

#! https://pybind11.readthedocs.io/en/stable/compiling.html



