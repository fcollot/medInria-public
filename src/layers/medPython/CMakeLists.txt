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

set(PYTHON_PACKAGE_NAME ${PROJECT_NAME})

set(RESOURCE_NAME ${TARGET_NAME}_RESOURCE)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  )

add_compile_definitions(
    PYTHON_PACKAGE_NAME="${PYTHON_PACKAGE_NAME}"
    LOCAL_BINDINGS_MODULE="${TARGET_NAME}_BINDINGS"
    GUI_BINDINGS_MODULE="${PROJECT_NAME}_GUI_BINDINGS"
    QT_BINDINGS_MODULE="${TARGET_NAME}_QT_BINDINGS"
    RESOURCE_FILE_NAME="${RESOURCE_NAME}.rcc"
    )

include(python_utils)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/objects
    ${CMAKE_CURRENT_SOURCE_DIR}/conversions
    ${CMAKE_CURRENT_SOURCE_DIR}/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

add_python_package(${PYTHON_PACKAGE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/python)

### #################################################################
### Python wrapping
### #################################################################

#add_python_bindings_module(${TARGET_NAME}_QT_BINDINGS
#    PACKAGE ${PYTHON_PACKAGE_NAME}

#    INCLUDE_INTERFACES
##    qstring.i
#    qwidget.i
#    )

#add_python_bindings_module(${TARGET_NAME}_BINDINGS
#    PACKAGE ${PYTHON_PACKAGE_NAME}
#    HEADER_FILE_PREFIX med

#    INCLUDE_HEADERS
##    Conversion.h

#    IMPORT_INTERFACES
#    ${${TARGET_NAME}_QT_BINDINGS_INTERFACE}

#    SIMPLE_BINDINGS
#    #ExternalPythonModules

#    DIRECTOR_BINDINGS
#    #PythonCommandLine
#    #PythonConsole
#    #PythonOutputStream
#    )

#add_python_bindings_module(${PROJECT_NAME}_GUI_BINDINGS
#    PACKAGE ${PYTHON_PACKAGE_NAME}
#    REMOVE_BINDINGS_PREFIX med

##    INCLUDE_HEADERS
##    medPython.h
##    Conversion.h

#    IMPORT_INTERFACES
#    ${${TARGET_NAME}_QT_BINDINGS_INTERFACE}

#    DIRECTOR_BINDINGS
#    medToolBox
#    )

add_python_resource(${RESOURCE_NAME} PACKAGES ${PYTHON_PACKAGE_NAME})

#set_target_properties(${TARGET_NAME} PROPERTIES RESOURCES ${PROJECT_NAME}_PYTHON)

## #############################################################################
## add library
## #############################################################################

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    ${SWIG_RUNTIME_HEADER} # allows access to the SWIG runtime
    )

## #############################################################################
## include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    ${SWIG_RUNTIME_HEADER_DIRECTORY}
    )

#target_include_directories(${TARGET_NAME}_QT_BINDINGS
#    PUBLIC
#    ${${TARGET_NAME}_INCLUDE_DIRS}
#    ${CMAKE_CURRENT_SOURCE_DIR}/swig
#    )

#target_include_directories(${TARGET_NAME}_BINDINGS
#    PUBLIC
#    ${${TARGET_NAME}_INCLUDE_DIRS}
#    ${CMAKE_CURRENT_SOURCE_DIR}/swig
#    )

#target_include_directories(${PROJECT_NAME}_GUI_BINDINGS
#    PUBLIC
#    ${${TARGET_NAME}_INCLUDE_DIRS}
#    ${CMAKE_CURRENT_SOURCE_DIR}/swig
#    )

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    ${Python3_LIBRARIES}
    medCoreLegacy
    )

#link_python_bindings_library(${TARGET_NAME}_QT_BINDINGS
#    Qt5::Core
#    ${TARGET_NAME}
#    )

#link_python_bindings_library(${TARGET_NAME}_BINDINGS
#    Qt5::Core
#    medCoreLegacy
#    ${TARGET_NAME}
#    )

#link_python_bindings_library(${PROJECT_NAME}_GUI_BINDINGS
#    Qt5::Core
#    medCoreLegacy
#    ${TARGET_NAME}
#    )

## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

#set_lib_install_rules(${TARGET_NAME}_QT_BINDINGS)

#set_lib_install_rules(${TARGET_NAME}_BINDINGS)

#set_lib_install_rules(${PROJECT_NAME}_GUI_BINDINGS)
