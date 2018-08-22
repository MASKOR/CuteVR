# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR module decoration

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

include(ConvertNamingStyle)

function(decorate_module _MODULE _SOURCES _LINK_PUBLIC _LINK_PRIVATE)
    if (NOT TARGET ${_MODULE})
        message(FATAL_ERROR "'${_MODULE}' is not a valid target.")
    endif ()

    # decorate the module with common properties
    add_library(CuteVR::${_MODULE} ALIAS ${_MODULE})
    if (_LINK_PUBLIC)
        add_dependencies(${_MODULE} ${_LINK_PUBLIC})
    endif ()
    set_target_properties(${_MODULE} PROPERTIES
                          VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                          FRAMEWORK ON
                          OUTPUT_NAME "CuteVR${_MODULE}"
                          CXX_STANDARD 14
                          CXX_STANDARD_REQUIRED ON
                          AUTOMOC ON)
    target_sources(${_MODULE} PRIVATE ${_SOURCES})
    target_include_directories(${_MODULE}
                               PUBLIC $<INSTALL_INTERFACE:include>
                               PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)
    target_link_libraries(${_MODULE}
                          PUBLIC ${_LINK_PUBLIC}
                          PRIVATE ${_LINK_PRIVATE})
    to_upper_snake_case(${_MODULE} _SNAKED)
    target_compile_definitions(${_MODULE} INTERFACE "CUTE_VR_${_SNAKED}")
endfunction()
