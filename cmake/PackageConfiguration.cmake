# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR package configuration

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

@PACKAGE_INIT@

set_and_check(CuteVR_DIR @PACKAGE_CMAKE_INSTALL_PREFIX@/lib/cmake/CuteVR)
set_and_check(CuteVR_INCLUDE_DIR @PACKAGE_CMAKE_INSTALL_PREFIX@/include)
set_and_check(CuteVR_LIBRARY_DIR @PACKAGE_CMAKE_INSTALL_PREFIX@/lib)

set(_SUPPORTED_COMPONENTS
    Core Audio AudioZeta Devices DevicesZeta Input InputZeta Render RenderZeta Video VideoZeta)

foreach (_COMPONENT IN LISTS CuteVR_FIND_COMPONENTS)
    if (NOT _COMPONENT IN_LIST _SUPPORTED_COMPONENTS)
        # component is not supported
        set(CuteVR_${_COMPONENT}_FOUND FALSE)
        string(REPLACE ";" ", " _SUPPORTED_COMPONENTS_READABLE "${_SUPPORTED_COMPONENTS}")
        string(CONCAT CuteVR_NOT_FOUND_MESSAGE "Specified unsupported component '${_COMPONENT}' of CuteVR. Supported "
               "are ${_SUPPORTED_COMPONENTS_READABLE}.")
        continue()
    elseif (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/${_COMPONENT}Config.cmake)
        # configuration file for component missing
        set(CuteVR_${_COMPONENT}_FOUND FALSE)
        string(CONCAT CuteVR_NOT_FOUND_MESSAGE "Configuration of specified component '${_COMPONENT}' not found under "
               "'${CMAKE_CURRENT_LIST_DIR}/${_COMPONENT}Config.cmake'.")
        continue()
    endif ()

    # determine if component is required or not
    if (CuteVR_FIND_REQUIRED AND CuteVR_FIND_REQUIRED_${_COMPONENT})
        set(_OPTION REQUIRED)
    elseif (CuteVR_FIND_QUIETLY OR NOT CuteVR_FIND_REQUIRED_${_COMPONENT})
        set(_OPTION QUIET)
    else ()
        message(FATAL_ERROR "Unexpected error due to component seems to be requested neither quiet nor required.")
    endif ()

    # configure package and test if target exists
    find_package(CuteVR_${_COMPONENT} ${_OPTION} CONFIG
                 NAMES ${_COMPONENT}
                 PATHS ${CMAKE_CURRENT_LIST_DIR}
                 NO_DEFAULT_PATH)
    if (NOT CuteVR_${_COMPONENT}_FOUND)
        set(CuteVR_NOT_FOUND_MESSAGE "Specified component '${_COMPONENT}' not found.")
    elseif (NOT TARGET CuteVR::${_COMPONENT})
        set(CuteVR_NOT_FOUND_MESSAGE "Target for specified component '${_COMPONENT}' unavailable.")
    endif ()
endforeach ()

check_required_components(CuteVR)
