# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR module testing

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

function(test_module _MODULE _SOURCES)
    if (NOT TARGET ${_MODULE})
        message(FATAL_ERROR "'${_MODULE}' is not a valid target.")
    elseif (NOT CuteVR_USE_UNIT_TESTS)
        return()
    endif ()

    find_package(Qt5 5.4.0 REQUIRED COMPONENTS Test)

    # add unit tests for the module
    foreach (_SOURCE IN LISTS _SOURCES)
        if (NOT _SOURCE)
            message(FATAL_ERROR "List of test sources contains empty element.")
        endif ()
        get_filename_component(_NAME "${_SOURCE}" NAME_WE)
        get_filename_component(_PREFIX "${_SOURCE}" DIRECTORY)
        string(REGEX REPLACE "^.*/test" "" _PREFIX "${_PREFIX}")
        string(REPLACE "/" "" _PREFIX "${_PREFIX}")
        set(_TEST "${_MODULE}${_PREFIX}${_NAME}")
        add_executable(${_TEST} "") # TODO: CMake 3.10 legacy, requires source files
        add_executable(CuteVR::${_TEST} ALIAS ${_TEST})
        add_dependencies(${_TEST} CuteVR::${_MODULE})
        set_target_properties(${_TEST} PROPERTIES
                              OUTPUT_NAME "CuteVR${_TEST}"
                              CXX_STANDARD 14
                              CXX_STANDARD_REQUIRED ON
                              AUTOMOC ON)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_sources(${_TEST} PRIVATE ${_SOURCE})
        target_include_directories(${_TEST}
                                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
                                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test)
        target_link_libraries(${_TEST}
                              PRIVATE Qt5::Core
                              PRIVATE Qt5::Test
                              PRIVATE CuteVR::${_MODULE})
        add_test(NAME ${_TEST} COMMAND ${_TEST})
        if (CuteVR_USE_UNIT_TESTS_POST_BUILD)
            add_custom_command(TARGET ${_TEST} POST_BUILD COMMAND ${_TEST})
        endif ()
    endforeach ()
endfunction()
