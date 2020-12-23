################################################################################
#
# medInria

# Copyright (c) INRIA 2013. All rights reserved.

# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt

# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

set(TARGET_NAME medPython)

## #############################################################################
## Find required
## #############################################################################

find_package(dtk REQUIRED)
include_directories(${dtk_INCLUDE_DIRS})

find_package(Qt5 REQUIRED Core)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(Python3 COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})

## #############################################################################
## List sources
## #############################################################################

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

file(GLOB ${TARGET_NAME}_SWIG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.i)

### #################################################################
### Python wrapping
### #################################################################

cmake_policy(SET CMP0078 NEW)

foreach(file ${${TARGET_NAME}_SWIG_FILES})
    set_source_files_properties(${file}
        PROPERTIES CPLUSPLUS ON
        USE_TARGET_INCLUDE_DIRECTORIES TRUE
        )
endforeach()

## #############################################################################
## add library
## #############################################################################

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    )

swig_add_library(${TARGET_NAME}_BINDINGS TYPE MODULE LANGUAGE python SOURCES
    ${${TARGET_NAME}_SWIG_FILES}
    ${${TARGET_NAME}_HEADERS}
    )

## #############################################################################
## include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

target_include_directories(${TARGET_NAME}_BINDINGS
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    ${Python3_LIBRARIES}
    medCoreLegacy
    )

swig_link_libraries(${TARGET_NAME}_BINDINGS
    Qt5::Core
    ${Python3_LIBRARIES}
    medCoreLegacy
    ${TARGET_NAME}
    )


## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )
