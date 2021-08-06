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

set(PARENT_TARGET ${TARGET_NAME})
set(TARGET_NAME ${PARENT_TARGET}Bindings)
set(PYTHON_PACKAGE ${PARENT_TARGET})

## #############################################################################
## List sources
## #############################################################################

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

## #############################################################################
## Add targets
## #############################################################################

add_library(${TARGET_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/../medPythonExport.h
    ${${TARGET_NAME}_CFILES}
    )

## #############################################################################
## Add Python modules
## #############################################################################

add_python_bindings(${TARGET_NAME} ${TARGET_NAME}_QT_BINDINGS
    INCLUDE_INTERFACES
    #    qstring.i
    qwidget.i
    )

add_python_bindings(${TARGET_NAME} ${TARGET_NAME}_CORE_BINDINGS
    DIRECTOR_BINDINGS
    medAbstractWorkspaceLegacy
    )

#generate_python_resources(${TARGET_NAME})

## #############################################################################
## Include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

target_include_directories(${TARGET_NAME}_QT_BINDINGS
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

target_include_directories(${TARGET_NAME}_CORE_BINDINGS
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
    PUBLIC
    Qt5::Core
    )

target_link_libraries(${TARGET_NAME}_QT_BINDINGS
    PUBLIC
    Qt5::Widgets
    )

target_link_libraries(${TARGET_NAME}_CORE_BINDINGS
    PUBLIC
    Qt5::Widgets
    medCoreLegacy
    )

## #############################################################################
## Install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )
