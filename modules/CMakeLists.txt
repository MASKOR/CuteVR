# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR modules

find_package(Qt5 5.4.0 REQUIRED COMPONENTS Core)

if (Qt5_VERSION VERSION_LESS 5.6)
    message(STATUS "Warning: You are using Qt ${Qt5_VERSION}. Support for older versions may be removed in the "
            "future, so we recommend upgrading to at least version 5.6.x.")
    execute_process(COMMAND sleep 3)
endif ()

cpack_add_component_group(Library EXPANDED)
cpack_add_component_group(Development)
cpack_add_component(Development DISABLED
                    GROUP Development)

# custom functions to configure modules
include(DecorateModule)
include(InstallModule)
include(TestModule)

# essential modules
list(APPEND _MODULES Core)

# add subdirectories of the selected modules
foreach (_MODULE IN LISTS _MODULES)
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_MODULE}/CMakeLists.txt)
        message(STATUS "Module '${_MODULE}' found.")
        add_subdirectory(./${_MODULE} ${CMAKE_CURRENT_BINARY_DIR}/${_MODULE})
    else ()
        message(STATUS "Module '${_MODULE}' not found.")
    endif ()
endforeach ()

# configure the package installation
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/CuteVRConfigVersion.cmake.in
                                 COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CuteVRConfigVersion.cmake.in
        DESTINATION lib/cmake/CuteVR
        COMPONENT Development
        RENAME CuteVRConfigVersion.cmake)
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/PackageConfiguration.cmake
                              ${CMAKE_CURRENT_BINARY_DIR}/CuteVRConfig.cmake.in
                              INSTALL_DESTINATION lib/cmake/CuteVR
                              INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
                              PATH_VARS CMAKE_INSTALL_PREFIX)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CuteVRConfig.cmake.in
        DESTINATION lib/cmake/CuteVR
        COMPONENT Development
        RENAME CuteVRConfig.cmake)
export(PACKAGE CuteVR)
