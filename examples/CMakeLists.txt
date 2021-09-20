message(STATUS "Building examples: ${CHEMICALFUN_BUILD_EXAMPLES}")

include_directories(${CMAKE_SOURCE_DIR})

set(Examples
        formula_parser_example
        elements_example )

foreach (exampleName ${Examples})
    add_executable(${exampleName} ${exampleName}.cpp)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
          target_link_libraries(${exampleName}
                PUBLIC ${PROJECT_NAME}
                PUBLIC pthread )
   elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

          target_link_libraries( ${exampleName}
                PUBLIC ${PROJECT_NAME}
                PUBLIC pthread )
    else()
          target_link_libraries( ${exampleName}
               PUBLIC ${PROJECT_NAME}
               PUBLIC pthread )
    endif()
endforeach()


