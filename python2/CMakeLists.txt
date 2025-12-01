message(STATUS "Building python library ")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# to test without install run into src: python test_elements.py
#file(COPY "${CMAKE_SOURCE_DIR}/examples/test_elements.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src")
#file(COPY "${CMAKE_SOURCE_DIR}/examples/test_elements2.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src")
#file(COPY "${CMAKE_SOURCE_DIR}/examples/test_generator.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src")

add_subdirectory(src/chemicalfun)

# Set the path where the python package is installed to CMAKE_INSTALL_PREFIX if not given
if(NOT DEFINED CHEMICALFUN_PYTHON_INSTALL_PREFIX)
    #if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
       # Determine the global site-packages directory
       # We execute a Python script to get this path reliably.
    #   execute_process(
    #     COMMAND "${Python3_EXECUTABLE}" -c "import sysconfig; print(sysconfig._getuserbase())"
    #     OUTPUT_VARIABLE CHEMICALFUN_PYTHON_INSTALL_PREFIX
    #     OUTPUT_STRIP_TRAILING_WHITESPACE
    #    )
    #    message(STATUS "!!!!!: ${CHEMICALFUN_PYTHON_INSTALL_PREFIX}")
    #else()
       set(CHEMICALFUN_PYTHON_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    #endif()
endif()

set(CHEMICALFUN_PYTHON_MODULE_FILENAME PyChemicalFun${PYTHON_MODULE_EXTENSION})

# If the path is already in Windows format (with backslashes), it can't be added directly
# to the string below, otherwise CMake will later complain about "Invalid escape sequence".
file(TO_CMAKE_PATH "${CHEMICALFUN_PYTHON_INSTALL_PREFIX}" CHEMICALFUN_PYTHON_INSTALL_PREFIX)
message(STATUS "CHEMICALFUN_PYTHON_INSTALL_PREFIX: ${CHEMICALFUN_PYTHON_INSTALL_PREFIX}")

# Install the ChemicalFun python package using setuptools
install(CODE
"
    file(TO_NATIVE_PATH \"${CHEMICALFUN_PYTHON_INSTALL_PREFIX}\" CHEMICALFUN_PYTHON_INSTALL_PREFIX_NATIVE)

   if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PyChemicalFun>
                ${CMAKE_CURRENT_BINARY_DIR}/src/chemicalfun/$<TARGET_FILE_NAME:PyChemicalFun>
      )
   endif()

    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/src/chemicalfun/PyChemicalFun.pdb)
        string(REPLACE .pyd .pdb CHEMICALFUN_PDB_FILENAME \"${CHEMICALFUN_PYTHON_MODULE_FILENAME}\")

        execute_process(
            COMMAND \${CMAKE_COMMAND} -E copy src/chemicalfun/PyChemicalFun.pdb \${CHEMICALFUN_PDB_FILENAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m pip install ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
"
)
#COMMAND ${Python3_EXECUTABLE} -m pip install --prefix \${CHEMICALFUN_PYTHON_INSTALL_PREFIX_NATIVE} ${CMAKE_CURRENT_BINARY_DIR}
