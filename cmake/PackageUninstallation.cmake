# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR package uninstallation

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt)
    message(FATAL_ERROR "Cannot find install manifest under '${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt'. "
            "Probably 'make install' has not been performed, or the manifest has been moved or deleted.")
endif ()

file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt _FILES)
foreach (_FILE IN LISTS _FILES)
    # test if file exists
    if (NOT EXISTS ${_FILE} AND NOT IS_SYMLINK ${_FILE})
        message(WARNING "File '${_FILE}' does not exist.")
        continue()
    elseif (IS_DIRECTORY ${_FILE})
        message(WARNING "'${_FILE}' is a directory.")
        continue()
    endif ()

    # remove file, and handle permission errors
    message(STATUS "Removing: '${_FILE}'")
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${_FILE}
                    RESULT_VARIABLE _RETURN)
    if (NOT ${_RETURN} EQUAL 0)
        message(SEND_ERROR "Failed to remove file: '${_FILE}' (Error: ${_RETURN}). You should check your write "
                "permissions and re-run the uninstallation.")
    endif ()

    # add the file path to the set of directories
    set(_DIRECTORY ${_FILE})
    while (TRUE)
        get_filename_component(_DIRECTORY ${_DIRECTORY} DIRECTORY)
        if (_DIRECTORY IN_LIST _DIRECTORIES OR NOT _DIRECTORY MATCHES CuteVR)
            break()
        else ()
            list(APPEND _DIRECTORIES ${_DIRECTORY})
        endif ()
    endwhile ()
endforeach ()

# make sure that subdirectories are listed first
list(SORT _DIRECTORIES)
list(REVERSE _DIRECTORIES)

foreach (_DIRECTORY IN LISTS _DIRECTORIES)
    # test if directory does not exists or is not empty
    file(GLOB _CONTENTS ${_DIRECTORY}/*)
    if (NOT EXISTS ${_DIRECTORY})
        message(WARNING "Directory '${_DIRECTORY}' does not exist.")
        continue()
    elseif (NOT IS_DIRECTORY ${_DIRECTORY})
        message(WARNING "'${_DIRECTORY}' is not a directory.")
        continue()
    elseif (_CONTENTS)
        message(WARNING "'${_DIRECTORY}' is not empty.")
        continue()
    endif ()

    # remove empty directories, and handle permission errors
    message(STATUS "Removing: '${_DIRECTORY}'")
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${_DIRECTORY}
                    RESULT_VARIABLE _RETURN)
    if (NOT ${_RETURN} EQUAL 0)
        message(SEND_ERROR "Failed to remove directory: '${_DIRECTORY}' (Error: ${_RETURN}). You should check your "
                "write permissions and re-run the uninstallation.")
    endif ()
endforeach ()
