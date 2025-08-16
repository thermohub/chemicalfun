# Find pybind11
#find_package(pybind11 REQUIRED)

# Build the Python wrappers of ChemicalFun library
add_subdirectory(PyChemicalFun)

# Build the Python package for ChemicalFun
add_subdirectory(chemicalfun)