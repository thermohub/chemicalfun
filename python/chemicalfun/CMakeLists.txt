# Copy the ChemicalFun directory to the binary directory
file(COPY . DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Define a variable with the name of the compiled python module file as defined by pybind11 (which is used in the file setup.py.in)
set(CHEMICALFUN_PYTHON_MODULE_FILENAME PyChemicalFun${PYTHON_MODULE_EXTENSION})
#message(STATUS "CHEMICALFUN_PYTHON_MODULE_FILENAME: ${CHEMICALFUN_PYTHON_MODULE_FILENAME}")

# Configure the setup.py file
#set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
#set(SETUP_PY ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

# Configure the setup.py script with the provided CMake variables
#configure_file(${SETUP_PY_IN} ${SETUP_PY})

# https://www.scivision.dev/cmake-install-python-package/
# https://pip.pypa.io/en/stable/topics/local-project-installs/#editable-installs
# detect virtualenv and set Pip args accordingly
if(DEFINED ENV{VIRTUAL_ENV} OR DEFINED ENV{CONDA_PREFIX})
  set(_PIP_ARG)
else()
  set(_PIP_ARG "--user")
endif()
#message(STATUS "_PIP_ARG: ${_PIP_ARG}")


# Create a custom target to build ChemicalFun python package during build stage
## Problem make with empty build generate error permissin and remove global setuped chemicalfun python lib
## when pip install try uninstall previos installed and if local not exist try to unistall from /usr/local\
## if installed to home/<usr>/.local.lib removes from here
add_custom_target(chemicalfun ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PyChemicalFun>
        ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:PyChemicalFun>
    #COMMAND ${Python3_EXECUTABLE} -m pip install ${_PIP_ARG} -e ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${Python3_EXECUTABLE} -m pip install --prefix ${CMAKE_BINARY_DIR} --ignore-installed .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set dependencies of PyChemicalFun target
add_dependencies(chemicalfun PyChemicalFun)

# Set the path where the python package is installed to CMAKE_INSTALL_PREFIX if not given
if(NOT DEFINED CHEMICALFUN_PYTHON_INSTALL_PREFIX)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
       # Determine the global site-packages directory
       # We execute a Python script to get this path reliably.
       execute_process(
         COMMAND "${Python3_EXECUTABLE}" -c "import site; print(site.getsitepackages()[0])"
         OUTPUT_VARIABLE CHEMICALFUN_PYTHON_INSTALL_PREFIX
         OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else()
       set(CHEMICALFUN_PYTHON_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    endif()
endif()

# If the path is already in Windows format (with backslashes), it can't be added directly
# to the string below, otherwise CMake will later complain about "Invalid escape sequence".
file(TO_CMAKE_PATH "${CHEMICALFUN_PYTHON_INSTALL_PREFIX}" CHEMICALFUN_PYTHON_INSTALL_PREFIX)
message(STATUS "CHEMICALFUN_PYTHON_INSTALL_PREFIX: ${CHEMICALFUN_PYTHON_INSTALL_PREFIX}")

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
        COMMAND ${Python3_EXECUTABLE} -m pip install --prefix \${CHEMICALFUN_PYTHON_INSTALL_PREFIX_NATIVE} ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
"
)
