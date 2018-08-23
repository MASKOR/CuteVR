# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR with examples

cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
cmake_policy(VERSION 3.5.1)

project("CuteVR"
        LANGUAGES CXX
        VERSION 0.8.0.0)
message(STATUS "*******  CuteVR  *******")

# check version recommendations
if (CMAKE_VERSION VERSION_LESS 3.9)
    message(STATUS "Warning: You are using CMake ${CMAKE_VERSION}. Support for older versions may be removed in the "
            "future, so we recommend upgrading to at least version 3.9.x.")
    execute_process(COMMAND sleep 3)
endif ()
if (CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.4)
        message(STATUS "Warning: You are using GCC ${CMAKE_CXX_COMPILER_VERSION}. This may work, but versions older "
                "than 5.4 are not officially supported. You should upgrade to a newer version.")
        execute_process(COMMAND sleep 3)
    elseif (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        message(STATUS "Warning: You are using GCC ${CMAKE_CXX_COMPILER_VERSION}. Support for older versions may be "
                "removed in the future, so we recommend upgrading to at least version 6.0.x.")
        execute_process(COMMAND sleep 3)
    endif ()
endif ()

# consider custom cmake find and config scripts
list(APPEND CMAKE_MODULE_PATH
     ${CMAKE_SOURCE_DIR}/cmake
     ${CMAKE_INSTALL_PREFIX}/lib/cmake)

# global variables
set(_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic"
    CACHE INTERNAL "Default C++ compiler flags.")
set(_BUNDLED ON
    CACHE INTERNAL "Bundled projects won't call find_package() and use local targets instead." FORCE)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # LEGACY: CMake 3.7 requires CMAKE_INCLUDE_CURRENT_DIR for moc

# package configuration
set(CPACK_PACKAGE_NAME "CuteVR")
set(CPACK_PACKAGE_VENDOR "Marcus Meeßen")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/package)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.md)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_CHECKSUM SHA512) # LEGACY: available from CMake 3.8 only
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON) # LEGACY: available from CMake 3.9 only

# macros and functions
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(CPack)
include(PrintOption)

# define general options
option(CuteVR_USE_OPEN_VR "OpenVR implementation will be used for hardware interaction." ON)
print_option(CuteVR_USE_OPEN_VR)
option(CuteVR_USE_DOXYGEN "Create a target that will run Doxygen on 'make documentation'." ON)
print_option(CuteVR_USE_DOXYGEN)
cmake_dependent_option(CuteVR_USE_DOXYGEN_POST_BUILD "Automatic execution of Doxygen after build is complete." OFF
                       "CuteVR_USE_DOXYGEN" OFF)
print_option(CuteVR_USE_DOXYGEN_POST_BUILD)
option(CuteVR_USE_LOCAL_INSTALLATION_PREFIX
       "The installation target is located in the build folder, so 'make install' shouldn't require admin privileges.")
print_option(CuteVR_USE_LOCAL_INSTALLATION_PREFIX)
option(CuteVR_USE_PREFERRED_CXX_FLAGS "The preferred flags to compile CuteVR are pedantic, namely '${_CXX_FLAGS}'." ON)
print_option(CuteVR_USE_PREFERRED_CXX_FLAGS)
option(CuteVR_USE_UNIT_TESTS "Build all available unit tests." ON)
print_option(CuteVR_USE_UNIT_TESTS)
cmake_dependent_option(CuteVR_USE_UNIT_TESTS_POST_BUILD "Automatic execution of unit tests after build is complete." ON
                       "CuteVR_USE_UNIT_TESTS" OFF)
print_option(CuteVR_USE_UNIT_TESTS_POST_BUILD)

# apply settings from options
if (CuteVR_USE_OPEN_VR)
    if (CMAKE_VERSION VERSION_LESS 3.12)
        add_definitions(-DCUTE_VR_OPEN_VR) # LEGACY: CMake 3.11 cannot add compile definitions directly
    else ()
        add_compile_definitions(CUTE_VR_OPEN_VR)
    endif ()
endif ()
if (CuteVR_USE_DOXYGEN)
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND) # LEGACY: CMake 3.8 cannot test on target Doxygen::doxygen
        if (NOT CuteVR_USE_DOXYGEN_POST_BUILD)
            set(_ALL)
            message(STATUS "CuteVR uses Doxygen to generate documentation.")
        else ()
            set(_ALL ALL)
            message(STATUS "CuteVR uses Doxygen to automatically generate documentation after the build is complete.")
        endif ()
        add_custom_target(documentation ${_ALL})
        add_custom_command(TARGET documentation POST_BUILD
                           COMMAND ${DOXYGEN_EXECUTABLE} "1>/dev/null" # LEGACY: CMake 3.8 cannot use Doxygen::doxygen
                           WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    else ()
        message(AUTHOR_WARNING "Doxygen cannot be found, make sure the software is properly installed. "
                "To remove this warning disable the 'CuteVR_USE_DOXYGEN_POST_BUILD' option again.")
    endif ()
endif ()
if (CuteVR_USE_LOCAL_INSTALLATION_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install
        CACHE PATH "Disable 'CuteVR_USE_LOCAL_INSTALLATION_PREFIX' to choose another path." FORCE)
    message(STATUS "CuteVR uses a local installation prefix, that is '${CMAKE_INSTALL_PREFIX}'.")
endif ()
if (CuteVR_USE_PREFERRED_CXX_FLAGS)
    string(APPEND CMAKE_CXX_FLAGS " ${_CXX_FLAGS}")
    message(STATUS "CuteVR uses the preferred c++ compile flags, namely '${_CXX_FLAGS}'.")
endif ()
if (CuteVR_USE_UNIT_TESTS)
    enable_testing()
    if (NOT CuteVR_USE_UNIT_TESTS_POST_BUILD)
        message(STATUS "CuteVR uses unit tests.")
    else ()
        message(STATUS "CuteVR automatically executes unit tests after build is complete.")
    endif ()
endif ()

# create target to uninstall the package
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/PackageUninstallation.cmake)
