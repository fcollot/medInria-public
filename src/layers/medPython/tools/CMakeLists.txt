################################################################################
#
# medInria
#
# Copyright (c) INRIA 2021. All rights reserved.
#
# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt
#
# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

set(TARGET_NAME ${PYTHON_PROJECT_NAME}Tools)

## #############################################################################
## List sources
## #############################################################################

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

file(GLOB swig_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/swig/*.i
    ${CMAKE_CURRENT_SOURCE_DIR}/swig/*.i.in)

file(GLOB python_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/python/*.py
    ${CMAKE_CURRENT_SOURCE_DIR}/python/*.py.in
    )

## #############################################################################
## Add targets
## #############################################################################

add_python_modules(${TARGET_NAME}
    PACKAGE ${TARGET_NAME}
    SOURCES ${python_sources} ${swig_sources} ${${TARGET_NAME}_CFILES}
    )

## #############################################################################
## Include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
    PUBLIC
    ${PYTHON_PROJECT_NAME}Base
    )
