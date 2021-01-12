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

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  )

include(medInria_python)

add_compile_definitions(TARGET_NAME="${TARGET_NAME}")

## #############################################################################
## Find required
## #############################################################################

find_package(dtk REQUIRED)
include_directories(${dtk_INCLUDE_DIRS})

find_package(Qt5 REQUIRED Core)

## #############################################################################
## List sources
## #############################################################################

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

### #################################################################
### Python wrapping
### #################################################################

add_python_package(${PROJECT_NAME} BINDINGS ${CMAKE_CURRENT_SOURCE_DIR}/swig/main.i.in)

add_python_resource(${PROJECT_NAME}_PYTHON)

set_target_properties(${TARGET_NAME} PROPERTIES RESOURCES ${PROJECT_NAME}_PYTHON)

## #############################################################################
## add library
## #############################################################################

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    )

## #############################################################################
## include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
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

get_python_binding_library_name(bindings_library ${TARGET_NAME} medInria)

link_python_binding_library(${bindings_library}
    Qt5::Core
    medCoreLegacy
    ${TARGET_NAME}
    )

## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )
