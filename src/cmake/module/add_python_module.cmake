################################################################################
#
# medInria
#
# Copyright (c) INRIA 2022. All rights reserved.
#
# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt
#
# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

set(PYTHON_MODULE_RESOURCE_DIR python/lib)

function(add_python_module target_name)

################################################################################
#
# Usage: add_python_module(target_name [PACKAGE package] [SOURCES [files...]])
#
# Adds a Python module or package target defined by a list of sources. The
# sources may specify pure Python modules (in which case they are simply copied
# to the resources folder), or SWIG interface files and C++ files that are used
# to generate Python bindings (which creates a pure Python module and an
# associated shared library). Python sources and SWIG interface files can be
# provided as configurable files (using the .in extension: py.in, i.in) in which
# case they will be automatically configured to generate .py and .i files. In
# all cases the PACKAGE option is required if the call specifies at least two
# pure Python modules (including the bindings module).
#
# The options are:
#
# PACKAGE
#     Specify the name of the package that will contain the modules. If this
#     option is used an __init__.py file should be included.
#
# SOURCES
#     Add python source files (.py, .py.in), Swig interface files (.i, .i.in),
#     and any necessary C++ files for the bindings library.
#
################################################################################

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "PACKAGE"
        "SOURCES"
        )

    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "No sources specified.")
    endif()

    _split_source_files(python_sources cpp_sources swig_sources ${ARG_SOURCES})

    set(resources_dir "${${PROJECT_NAME}_BINARY_RESOURCE_DIR}/${PYTHON_MODULE_RESOURCE_DIR}")

    if(ARG_PACKAGE)
        string(REGEX REPLACE "\\." "/" package_dir ${ARG_PACKAGE})
        string(APPEND resources_dir "/${package_dir}")
    endif()

    if(swig_sources)
        set(swig_args
            RESOURCES_DIR ${resources_dir}
            SWIG_SOURCES ${swig_sources}
            )

        if(cpp_sources)
            list(APPEND swig_args CPP_SOURCES ${cpp_sources})
        endif()

        if(ARG_PACKAGE)
            list(APPEND swig_args PACKAGE ${ARG_PACKAGE})
        endif()

        _add_bindings_module(${target_name} ${swig_args})
    endif()

    if(python_sources)
        if(swig_sources)
            set(copy_target ${target_name}_file_copy)
        else()
            set(copy_target ${target_name})
        endif()

        add_custom_target(${copy_target} ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory "${resources_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${python_sources} "${resources_dir}"
            DEPENDS ${python_sources}
            )
    endif()

endfunction()

macro(_split_source_files python_files_var cpp_files_var swig_files_var)

    set(${python_files_var})
    set(${cpp_files_var})
    set(${swig_files_var})

    foreach(file ${ARGN})
        get_filename_component(extension "${file}" EXT)
        get_filename_component(filename "${file}" NAME_WE)

        if(extension STREQUAL ".py")
            list(APPEND ${python_files_var} "${file}")
        elseif (extension STREQUAL ".py.in")
            configure_file("${file}" "${CMAKE_CURRENT_BINARY_DIR}/${filename}.py" @ONLY)
            list(APPEND ${python_files_var} "${CMAKE_CURRENT_BINARY_DIR}/${filename}.py")
        elseif (extension STREQUAL ".i")
            list(APPEND ${swig_files_var} "${file}")
        elseif (extension STREQUAL ".i.in")
            configure_file("${file}" "${CMAKE_CURRENT_BINARY_DIR}/swig/${filename}.i" @ONLY)
            list(APPEND ${swig_files_var} "${CMAKE_CURRENT_BINARY_DIR}/swig/${filename}.i")
        else()
            list(APPEND ${cpp_files_var} "${file}")
        endif()
    endforeach()

endmacro()

function(_add_bindings_module target_name)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "PACKAGE;RESOURCES_DIR"
        "SWIG_SOURCES;CPP_SOURCES"
        )

    set(working_dir "${CMAKE_CURRENT_BINARY_DIR}/swig")
    set(main_swig_source)

    foreach(file ${ARG_SWIG_SOURCES})
        file(STRINGS "${file}" file_content LIMIT_COUNT 1)

        if(file_content)
            list(GET file_content 0 first_line)

            if(first_line MATCHES "%module.*\(.*\).*([^ ]+)$")
                set(main_swig_source "${file}")
                break()
            endif()
        endif()
    endforeach()

    if(NOT main_swig_source)
        message(FATAL_ERROR "No \"%module\" directive found in swig sources (must be on first line).")
    endif()

    set_source_files_properties("${main_swig_source}" PROPERTIES
        CPLUSPLUS ON
        USE_SWIG_DEPENDENCIES ON
        )

    swig_add_library(${target_name}
        TYPE SHARED
        LANGUAGE python
        OUTPUT_DIR "${ARG_RESOURCES_DIR}"
        OUTFILE_DIR "${working_dir}"
        SOURCES ${main_swig_source} ${ARG_CPP_SOURCES}
        )

    set_target_properties(${target_name} PROPERTIES
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE
        SWIG_COMPILE_DEFINITIONS SWIG_TYPE_TABLE=${PYTHON_PROJECT_NAME}
        SWIG_COMPILE_OPTIONS -py3
        OUTPUT_NAME ${target_name}
        )

    target_sources(${target_name} PRIVATE ${ARG_SWIG_SOURCES} ${ARG_CPP_SOURCES})

    set_lib_install_rules(${target_name} RESOURCE)

    if(ARG_PACKAGE)
        get_target_property(prefix ${target_name} PREFIX)
        set(forward_module "${ARG_RESOURCES_DIR}/${prefix}${target_name}.py")
        file(WRITE ${forward_module} "from ${prefix}${target_name} import *")
    endif()

endfunction()
