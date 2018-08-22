# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR convert between different naming styles

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(to_snake_case _STRING _RETURN)
    string(REGEX MATCHALL "[^a-zA-Z0-9]+" _FORBIDDEN_CHARACTERS "${_STRING}")
    if (_FORBIDDEN_CHARACTERS)
        message(FATAL_ERROR "String contains forbidden characters '${_FORBIDDEN_CHARACTERS}'.")
    endif ()
    string(REGEX REPLACE "([0-9]+|[A-Z]?[a-z]+)" "_\\1" _STRING "${_STRING}")
    string(REGEX REPLACE "^_" "" _STRING "${_STRING}")
    set(${_RETURN} ${_STRING} PARENT_SCOPE)
endfunction()

function(to_upper_snake_case _STRING _RETURN)
    to_snake_case("${_STRING}" _SNAKED)
    string(TOUPPER "${_SNAKED}" _STRING)
    set(${_RETURN} ${_STRING} PARENT_SCOPE)
endfunction()

function(to_lower_snake_case _STRING _RETURN)
    to_snake_case("${_STRING}" _SNAKED)
    string(TOLOWER "${_SNAKED}" _STRING)
    set(${_RETURN} ${_STRING} PARENT_SCOPE)
endfunction()

function(to_camel_case _STRING _RETURN)
    string(REGEX MATCHALL "[^a-zA-Z0-9_]+" _FORBIDDEN_CHARACTERS "${_STRING}")
    if (_FORBIDDEN_CHARACTERS)
        message(FATAL_ERROR "String contains forbidden characters '${_FORBIDDEN_CHARACTERS}'.")
    endif ()
    string(REGEX MATCHALL "." _LETTERS "${_STRING}")
    foreach (_LETTER IN LISTS _LETTERS)
        if (NOT _LETTER)
            message(FATAL_ERROR "Unexpected error due to letter seems to be empty.")
        endif ()
        if (_LETTER STREQUAL "_")
            set(_HUMP TRUE)
        else ()
            if (_HUMP)
                set(_HUMP FALSE)
                string(TOUPPER ${_LETTER} _LETTER)
            else ()
                string(TOLOWER ${_LETTER} _LETTER)
            endif ()
            string(APPEND _CAMELED ${_LETTER})
        endif ()
    endforeach ()
    set(${_RETURN} ${_CAMELED} PARENT_SCOPE)
endfunction()

function(to_capitalized_camel_case _STRING _RETURN)
    to_camel_case("_${_STRING}" _CAMELED)
    set(${_RETURN} ${_CAMELED} PARENT_SCOPE)
endfunction()
