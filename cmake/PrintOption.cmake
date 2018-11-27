# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR nice printout for options

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(print_option _OPTION)
    message(STATUS "Option '${_OPTION}' is '${${_OPTION}}'.")
endfunction()
