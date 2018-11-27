# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR module installation

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

function(install_module _MODULE _DEPENDENCIES _EXCLUDE_REGEX)
    if (NOT TARGET ${_MODULE})
        message(FATAL_ERROR "'${_MODULE}' is not a valid target.")
    endif ()

    # install and export the module itself
    set(_CONFIG "${_MODULE}Config")
    set(_EXPORT "${_MODULE}Export")
    install(TARGETS ${_MODULE}
            EXPORT ${_EXPORT}
            ARCHIVE
            DESTINATION lib
            COMPONENT ${_MODULE}
            LIBRARY
            DESTINATION lib
            COMPONENT ${_MODULE})
    export(EXPORT ${_EXPORT}
           NAMESPACE CuteVR::)

    # compose commands to find the packages of dependencies
    foreach (_DEPENDENCY IN LISTS _DEPENDENCIES)
        if (NOT _DEPENDENCY)
            message(FATAL_ERROR "List of dependencies contains empty element.")
        endif ()
        string(REGEX REPLACE "[^a-zA-Z0-9_:]+" "" _DEPENDENCY "${_DEPENDENCY}")
        string(REPLACE "::" ";" _SPLIT "${_DEPENDENCY}")
        list(LENGTH _SPLIT _LENGTH)
        if (_LENGTH GREATER 0)
            list(GET _SPLIT 0 _PACKAGE)
            string(STRIP "${_PACKAGE}" _PACKAGE)
            if (NOT _PACKAGE)
                message(FATAL_ERROR "Package name of dependency is emtpy.")
            endif ()
        endif ()
        if (_LENGTH EQUAL 1)
            string(APPEND _FIND_DEPENDENCIES "find_package(${_PACKAGE} REQUIRED)\n")
        elseif (_LENGTH EQUAL 2)
            list(GET _SPLIT 1 _COMPONENT)
            string(STRIP "${_COMPONENT}" _COMPONENT)
            if (NOT _COMPONENT)
                message(FATAL_ERROR "Component name of dependency is emtpy.")
            endif ()
            string(APPEND _FIND_DEPENDENCIES "find_package(${_PACKAGE} REQUIRED COMPONENTS ${_COMPONENT})\n")
        endif ()
    endforeach ()
    string(REGEX REPLACE "\n$" "" _FIND_DEPENDENCIES ${_FIND_DEPENDENCIES})

    # install development stuff and header files
    configure_file(${CMAKE_SOURCE_DIR}/cmake/ModuleConfiguration.cmake
                   ${CMAKE_CURRENT_BINARY_DIR}/${_CONFIG}.cmake.in @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_CONFIG}.cmake.in
            DESTINATION lib/cmake/CuteVR
            COMPONENT Development
            RENAME ${_CONFIG}.cmake)
    install(EXPORT ${_EXPORT}
            DESTINATION lib/cmake/CuteVR
            COMPONENT Development
            NAMESPACE CuteVR::)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/CuteVR
            DESTINATION include
            COMPONENT Development
            FILES_MATCHING PATTERN "*.hpp"
            REGEX "${_EXCLUDE_REGEX}" EXCLUDE)
endfunction()
