
# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES_TOP  ${CMAKE_SOURCE_DIR}/include/ChemicalFun/*.h )

# Recursively collect all header files from the current directory
file(GLOB_RECURSE HEADER_FILES  *.h )

# Recursively collect all source files from the current directory
file(GLOB_RECURSE SOURCE_FILES  *.cpp )

# The name of the shared and static libraries
set(CHEMICALFUN_SHARED_LIB ${PROJECT_NAME}${SUFFIX_SHARED_LIBS})
set(CHEMICALFUN_STATIC_LIB ${PROJECT_NAME}${SUFFIX_STATIC_LIBS})

# Enable automatic creation of a module definition (.def) file for a SHARED library on Windows.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Build a ChemicalFun C++ library
add_library(ChemicalFun ${HEADER_FILES_TOP} ${HEADER_FILES} ${SOURCE_FILES})

# Add an alias ChemicalFun::ChemicalFun to the target library ChemicalFun
add_library(ChemicalFun::ChemicalFun ALIAS ChemicalFun)

#target_compile_definitions(ChemicalFun PUBLIC -DSPDLOG_FMT_EXTERNAL=yes)

#target_link_libraries(ChemicalFun PUBLIC spdlog::spdlog fmt::fmt)
target_link_libraries(ChemicalFun PUBLIC spdlog::spdlog)

# Add the include paths to the ReactionsGenerator target
target_include_directories(ChemicalFun
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Create install target for library
install(TARGETS ChemicalFun
    EXPORT ChemicalFunTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libraries)

# Install debug symbols
if(MSVC)
    install(
        FILES $<TARGET_PDB_FILE:ChemicalFun>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT libraries
        OPTIONAL
    )
endif()

# Install the header files preserving the directory hierarchy
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ChemicalFun
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

