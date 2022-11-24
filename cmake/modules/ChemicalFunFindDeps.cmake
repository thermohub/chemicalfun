
# Find pybind11 library (if needed)
if(CHEMICALFUN_BUILD_PYTHON)
    find_package(pybind11 REQUIRED)
    if(NOT pybind11_FOUND)
        message(WARNING "Could not find pybind11 - the Python module `chemicalfun` will not be built.")
        set(CHEMICALFUN_BUILD_PYTHON OFF)
    else()
        message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
    endif()
endif()


find_package(spdlog REQUIRED)
if(NOT spdlog_FOUND)
  message(FATAL_ERROR "spdlog not found")
else()
  message(STATUS "Found spdlog v${spdlog_VERSION}")
endif()


find_package(fmt REQUIRED)
if(NOT fmt_FOUND)
  message(FATAL_ERROR "fmt not found")
else()
  message(STATUS "Found fmt v${fmt_VERSION}")
endif()
