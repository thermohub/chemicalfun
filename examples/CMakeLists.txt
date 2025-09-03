message(STATUS "Building examples: ${CHEMICALFUN_BUILD_EXAMPLES}")

include_directories(${CMAKE_SOURCE_DIR})

set(Examples
        formula_parser_example
        elements_example
        dbelements_example
        thread_example )

foreach (exampleName ${Examples})
    add_executable(${exampleName} ${exampleName}.cpp)
	if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
		target_link_libraries(${exampleName}
         PUBLIC ChemicalFun)
		 # We don't have or need pthread in the standard MSVC kit.
	else()
		target_link_libraries(${exampleName}
         PUBLIC ChemicalFun
         PUBLIC pthread)
	endif()
    
endforeach()


