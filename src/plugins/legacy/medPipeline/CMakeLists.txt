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

set(TARGET_NAME medPipelinePlugin)
set(PYTHON_PACKAGE_NAME medPipeline)

set(${TARGET_NAME}_VERSION ${${PROJECT_NAME}_VERSION})

include(python_utils)

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
    ${${TARGET_NAME}_CFILES}
    )

target_compile_definitions(${TARGET_NAME} PRIVATE
    TARGET_NAME="${TARGET_NAME}"
    TARGET_VERSION="${${TARGET_NAME}_VERSION}"
    PYTHON_PACKAGE_NAME="${PYTHON_PACKAGE_NAME}"
    )

## #############################################################################
## Add Python modules
## #############################################################################

set_property(TARGET ${TARGET_NAME} PROPERTY
    PYTHON_PACKAGE_NAME ${PYTHON_PACKAGE_NAME})

add_python_modules(${TARGET_NAME}
    SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
    __init__.py
    selection_toolbox.py
    workspace.py
    )

generate_python_resources(${TARGET_NAME})

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

## #############################################################################
## Install
## #############################################################################

set_plugin_install_rules_legacy(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )
