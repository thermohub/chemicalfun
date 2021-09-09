# The path where cmake config files are installed
set(CHEMICALFUN_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ChemicalFun)

install(EXPORT ChemicalFunTargets
    FILE ChemicalFunTargets.cmake
    NAMESPACE ChemicalFun::
    DESTINATION ${CHEMICALFUN_INSTALL_CONFIGDIR}
    COMPONENT cmake)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ChemicalFunConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/ChemicalFunConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ChemicalFunConfig.cmake
    INSTALL_DESTINATION ${CHEMICALFUN_INSTALL_CONFIGDIR}
    PATH_VARS CHEMICALFUN_INSTALL_CONFIGDIR)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ChemicalFunConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ChemicalFunConfigVersion.cmake
    DESTINATION ${CHEMICALFUN_INSTALL_CONFIGDIR})
