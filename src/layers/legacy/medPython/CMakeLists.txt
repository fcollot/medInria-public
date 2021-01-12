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
set(TARGET_BINDINGS ${TARGET_NAME}_BINDINGS)

add_compile_definitions(TARGET_NAME="${TARGET_NAME}")
add_compile_definitions(TARGET_BINDINGS="${TARGET_BINDINGS}")

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

file(GLOB_RECURSE ${TARGET_NAME}_PYTHON_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/python
    python/*.py
    python/*.py.in
    )

file(GLOB_RECURSE ${TARGET_BINDINGS}_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/bindings
    bindings/*.i
    bindings/*.i.in
    )

### #################################################################
### Python wrapping
### #################################################################

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

set(RESOURCE_FILES)

foreach(file ${${TARGET_NAME}_PYTHON_FILES})
    set(new_file ${CMAKE_CURRENT_BINARY_DIR}/python/${TARGET_NAME}/${file})

    if(new_file MATCHES ".*\.in")
        string(REPLACE ".in" "" new_file ${new_file})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/python/${file} ${new_file})
    else()
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/python/${file} ${new_file} COPYONLY)
    endif()

    get_filename_component(module_dir ${file} DIRECTORY)
    get_filename_component(module_name ${file} NAME_WE)
    if (module_dir)
        string(APPEND module_dir "/")
    endif()
    list(APPEND RESOURCE_FILES <file>python/${TARGET_NAME}/${module_dir}${module_name}.py</file>)
endforeach()

foreach(file ${${TARGET_BINDINGS}_FILES})
    set(new_file ${CMAKE_CURRENT_BINARY_DIR}/bindings/${file})

    if(new_file MATCHES ".*\.in")
        string(REPLACE ".in" "" new_file ${new_file})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bindings/${file} ${new_file})
    else()
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bindings/${file} ${new_file} COPYONLY)
    endif()

    set_source_files_properties(${new_file}
        PROPERTIES CPLUSPLUS ON
        USE_TARGET_INCLUDE_DIRECTORIES TRUE
        )
endforeach()

if(${TARGET_BINDINGS}_FILES)
    set(bindings_module python/${TARGET_NAME}/${TARGET_BINDINGS}.py)
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/${bindings_module})
    list(APPEND RESOURCE_FILES <file>${bindings_module}</file>)
endif()

string(REPLACE ";" " " RESOURCE_FILES ${RESOURCE_FILES})
configure_file(resources.qrc.in ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc)

## #############################################################################
## add library
## #############################################################################

swig_add_library(${TARGET_BINDINGS}
    TYPE MODULE
    LANGUAGE python
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/python/${TARGET_NAME}
    SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/bindings/main.i
    )

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    )

qt5_add_binary_resources(${TARGET_NAME}_RESOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc
    DESTINATION ${CMAKE_BINARY_DIR}/resources/${TARGET_NAME}.rcc # must create dir
    )

add_dependencies(${TARGET_NAME}_RESOURCES ${TARGET_BINDINGS}_swig_compilation)

set_target_properties(${TARGET_NAME} PROPERTIES RESOURCES ${TARGET_NAME}_RESOURCES)

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

swig_link_libraries(${TARGET_BINDINGS}
    Qt5::Core
    ${Python3_LIBRARIES}
    medCoreLegacy
    ${TARGET_NAME}
    )

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    ${Python3_LIBRARIES}
    medCoreLegacy
    )


## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

set_lib_install_rules(${TARGET_BINDINGS})
