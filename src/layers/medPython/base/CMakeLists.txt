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

set(TARGET_NAME ${PYTHON_PROJECT_NAME}Base)

## #############################################################################
## List sources
## #############################################################################

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/conversion
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/error
    ${CMAKE_CURRENT_SOURCE_DIR}/objects
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

## #############################################################################
## Generate SWIG Runtime header
## #############################################################################

set(SWIG_RUNTIME_HEADER_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/swig)
set(SWIG_RUNTIME_HEADER ${SWIG_RUNTIME_HEADER_DIRECTORY}/swig_runtime.h)

add_custom_command(
    OUTPUT ${SWIG_RUNTIME_HEADER}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SWIG_RUNTIME_HEADER_DIRECTORY}
    COMMAND ${SWIG_EXECUTABLE}
    ARGS
    "-python"
    "-c++"
    "-external-runtime"
    ${SWIG_RUNTIME_HEADER}
    )

## #############################################################################
## Add targets
## #############################################################################

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    ${SWIG_RUNTIME_HEADER}
    )

target_compile_definitions(${TARGET_NAME} PRIVATE
    TARGET_NAME="${TARGET_NAME}"
    PYTHON_PLUGIN_PREFIX="${PYTHON_PLUGIN_PREFIX}"
    PROJECT_NAME="${PROJECT_NAME}"
    PYTHON_VERSION_STRING="${python_VERSION_MAJOR}.${python_VERSION_MINOR}"
    )

## #############################################################################
## Include directories
## #############################################################################

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    "${SWIG_RUNTIME_HEADER_DIRECTORY}"
    "${python_SOURCE_DIR}/Include"
    "${python_BINARY_DIR}"
    )

## #############################################################################
## Embed Python
## #############################################################################

set(python_vname python${python_VERSION_MAJOR}.${python_VERSION_MINOR})
set(python_resource_dir ${python_vname}/lib/${python_vname})

## #############################################################################
## Import standard Python modules
## #############################################################################

file(GLOB modules ${python_SOURCE_DIR}/Lib/*)

set(files)
set(directories)

foreach (module ${modules})
    if (IS_DIRECTORY "${module}")
        get_filename_component(module_name "${module}" NAME)
        set_external_resources(${TARGET_NAME} APPEND
            DESTINATION ${python_resource_dir}/${module_name}
            DIRECTORIES ${module}
            )
    else()
        list(APPEND files "${module}")
    endif()
endforeach()

set_external_resources(${TARGET_NAME} APPEND DESTINATION ${python_resource_dir} FILES ${files})

## #############################################################################
## Import standard Python libraries
## #############################################################################

file(READ "${python_BINARY_DIR}/pybuilddir.txt" pybuilddir)
set_external_resources(${TARGET_NAME} APPEND DESTINATION ${python_resource_dir}/lib-dynload DIRECTORIES "${python_BINARY_DIR}/${pybuilddir}")

## #############################################################################
## Import core Python libraries
## #############################################################################

if (UNIX)
    if (APPLE)
        set(python_library_names lib${python_vname}.dylib)
    else()
        set(python_library_names lib${python_vname}.so)
    endif()
elseif (WIN32)
endif()

set(python_libraries)

foreach (library_name ${python_library_names})
    set(source_library "${python_BINARY_DIR}/${library_name}")
    set(dest_library "${CMAKE_BINARY_DIR}/lib/${library_name}")

    if (APPLE)
        add_custom_command(OUTPUT "${dest_library}"
            COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${source_library}" "${dest_library}"
            COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "${source_library}" "${dest_library}"
            )
    else()
        add_custom_command(OUTPUT "${dest_library}"
            COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${source_library}" "${dest_library}"
            )
    endif()

    install(FILES "${dest_library}" TYPE LIB)
    list(APPEND python_libraries "${dest_library}")
endforeach()

add_custom_target(import_core_python_libraries DEPENDS ${python_libraries})

add_dependencies(${TARGET_NAME} import_core_python_libraries)

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
    PUBLIC
    Qt5::Core
    medCoreLegacy
    )

if (NOT WIN32)
    target_link_libraries(${TARGET_NAME} PUBLIC "${python_libraries}")
endif()

## #############################################################################
## Install
## #############################################################################

set_lib_install_rules(${TARGET_NAME} HEADERS
    ${${TARGET_NAME}_HEADERS}
    )
