# Copy the ChemicalFun directory to the binary directory
file(COPY . DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Define a variable with the name of the compiled python module file as defined by pybind11 (which is used in the file setup.py.in)
set(CHEMICALFUN_PYTHON_MODULE_FILENAME PyChemicalFun${PYTHON_MODULE_EXTENSION})

# Configure the setup.py file
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

# Configure the setup.py script with the provided CMake variables
configure_file(${SETUP_PY_IN} ${SETUP_PY})

# Create a custom target to build ChemicalFun python package during build stage
add_custom_target(chemicalfun ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PyChemicalFun>
        ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:PyChemicalFun>
    COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} --quiet install --prefix=${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set dependencies of PyChemicalFun target
add_dependencies(chemicalfun PyChemicalFun)

# Set the path where the python package is installed to CMAKE_INSTALL_PREFIX if not given
if(NOT DEFINED CHEMICALFUN_PYTHON_INSTALL_PREFIX)
    set(CHEMICALFUN_PYTHON_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()
# If the path is already in Windows format (with backslashes), it can't be added directly
# to the string below, otherwise CMake will later complain about "Invalid escape sequence".
file(TO_CMAKE_PATH "${CHEMICALFUN_PYTHON_INSTALL_PREFIX}" CHEMICALFUN_PYTHON_INSTALL_PREFIX)

# Install the ChemicalFun python package using setuptools
install(CODE
"
    file(TO_NATIVE_PATH \"${CHEMICALFUN_PYTHON_INSTALL_PREFIX}\" CHEMICALFUN_PYTHON_INSTALL_PREFIX_NATIVE)

    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../../lib/PyChemicalFun.pdb)
        string(REPLACE .pyd .pdb CHEMICALFUN_PDB_FILENAME \"${CHEMICALFUN_PYTHON_MODULE_FILENAME}\")

        execute_process(
            COMMAND \${CMAKE_COMMAND} -E copy ../../lib/PyChemicalFun.pdb \${CHEMICALFUN_PDB_FILENAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --prefix=\${CHEMICALFUN_PYTHON_INSTALL_PREFIX_NATIVE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
"
)

# https://www.scivision.dev/cmake-install-python-package/
# https://github.com/pybind/cmake_example
#https://medium.com/practical-coding/setting-up-a-c-python-project-with-pybind11-and-cmake-8de391494fca
# https://github.com/smrfeld/cmake_cpp_pybind11_tutorial
#  https://stackoverflow.com/questions/45225381/pybind11-cmake-how-to-install-files
