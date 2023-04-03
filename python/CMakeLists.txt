################################################################################
#
# medInria
#
# Copyright (c) INRIA. All rights reserved.
#
# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt
#
# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

cmake_minimum_required(VERSION 3.19)

include(CMakePackageConfigHelpers)
include(FetchContent)

project(medInria_python VERSION ${medInria_python_VERSION})

## #############################################################################
## Settings
## #############################################################################

set(${PROJECT_NAME}_PYTHON_PACKAGES
    numpy
    CACHE STRING "Third-party Python packages to embed"
    )

set(${PROJECT_NAME}_SITE_PATH "${CMAKE_BINARY_DIR}/site-packages")

## #############################################################################
## PYNCPP
## #############################################################################

FetchContent_Declare(PYNCPP
    # GIT_REPOSITORY ${GITHUB_PREFIX}LIRYC-IHU/pyncpp.git
    # GIT_TAG origin/working
    GIT_REPOSITORY ${GITHUB_PREFIX}fcollot/pyncpp.git
    GIT_TAG origin/pack4
    GIT_SHALLOW True
    GIT_PROGRESS True
    )

FetchContent_MakeAvailable(PYNCPP)

#find_package(PYNCPP REQUIRED HINTS ${PYNCPP_BINARY_DIR})

## #############################################################################
## Additional Python packages
## #############################################################################

#PYNCPP_pip(python_packages
#    DIRECTORY "${${PROJECT_NAME}_SITE_PATH}"
#    COMMAND install --upgrade ${${PROJECT_NAME}_PYTHON_PACKAGES})

## #############################################################################
## Config files
## #############################################################################

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/medInria_pythonConfig.cmake.in" "${PROJECT_BINARY_DIR}/medInria_pythonConfig.cmake"
    INSTALL_PREFIX "${PROJECT_BINARY_DIR}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
    PATH_VARS
    PYNCPP_DIR
    )

write_basic_package_version_file("${PROJECT_BINARY_DIR}/medInria_pythonConfigVersion.cmake"
    VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
    COMPATIBILITY SameMinorVersion
    )

## #############################################################################
## Install
## #############################################################################

#if(APPLE)
#    set(app_destination " bin/medInria.app/Contents/MacOS")
#    set(lib_destination "bin/medInria.app/Contents/Frameworks")
#    set(python_destination "bin/medInria.app/Contents/Resources")
#elseif(WIN32)
#    set(app_destination "bin")
#    set(lib_destination "bin")
#    set(python_destination "bin")
#else()
#    set(app_destination "bin")
#    set(lib_destination "lib")
#    set(python_destination "lib")
#endif()

#PYNCPP_install_libraries(
#    DESTINATION "${lib_destination}"
#    COMPONENT Runtime
#    )

#PYNCPP_install_path_config_file(
#    APPLICATION_NAME "medInria"
#    DESTINATION "${app_destination}"
#    PYTHON_PATH "${python_destination}"
#    COMPONENT Runtime
#    )

#PYNCPP_install_python(
#    DESTINATION "${python_destination}"
#    EXECUTABLE TRUE
#    COMPONENT Runtime
#    )

#PYNCPP_install_python_modules(
#    DIRECTORY "${${PROJECT_NAME}_SITE_PATH}"
#    DESTINATION "${python_destination}"
#    COMPONENT Runtime
#    )
