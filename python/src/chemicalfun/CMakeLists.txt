message(STATUS "Building python library ")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Collect the source files of PyChemicalData
file(GLOB_RECURSE HEADER_FILES *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

include_directories(${CONDA_AWARE_PREFIX}/include/eigen3)
include_directories("/usr/local/include/eigen3")

# Create the PyChemicalData library
pybind11_add_module(PyChemicalFun ${SOURCE_FILES} NO_EXTRAS)
# NO_EXTRAS used to decrease linking time, but causes larger module file size

# Link PyChemicalData against ChemicalData library
target_link_libraries(PyChemicalFun PUBLIC ChemicalFun::ChemicalFun)

# https://pybind11.readthedocs.io/en/stable/compiling.html

# https://packaging.python.org/en/latest/guides/writing-pyproject-toml/

# https://setuptools.pypa.io/en/stable/userguide/datafiles.html
