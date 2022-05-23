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

function(add_python_modules target_name)

################################################################################
#
# Usage: add_python_modules([PACKAGE package] [SOURCES [files...]])
#
# Creates a target that provides python modules (these modules will need to be
# imported through `import_external_resources`, see that function for more
# details). The modules can be specified as .py and .i files (the latter will
# generate a bindings module through Swig), and also .py.in and .i.in files
# (these will be automatically configured to generate .py and .i files).
#
# If a bindings module is created, the corresponding bindings library will be
# placed with the normal libraries (not in the resources).
#
# The options are:
#
# PACKAGE
#     Specify the name of the package that will contain the modules. This option
#     is required if more than one modules are provided, and optional for a
#     a single module.
#
# PARENT_PACKAGE
#     Specify if the module (or package) is inside another package. The value of
#     this option must be a target previously created with `add_python_modules`.
#
# BINDINGS_MODULE_NAME
#     Specify the name of the generated bindings module. The default name is
#     "bindings.py" if this command creates a package, or the target name if it
#     does not.
#
# BINDINGS_LIBRARY_NAME
#     Specify the name of the generated bindings library. The default name is
#     the same is the bindings module with one exception: if the module name was
#     not provided and we are creating a package, then the library name will be
#     [package name]_bindings.
#
# SOURCES
#     Add python source files (.py, .py.in) and Swig interface files (.i, .i.in).
#     The interface files result in the generation of one python module and one
#     shared library.
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

    get_external_resources_directory(resources_dir)
    string(APPEND resources_dir "/python/lib")

    if(ARG_PACKAGE)
        string(REGEX REPLACE "\\." "/" package_dir ${ARG_PACKAGE})
        string(APPEND resources_dir "/${package_dir}")
    endif()

    if(swig_sources)
        if(${ARG_PACKAGE})
            set(package_option "PACKAGE TRUE")
        endif()

        _add_bindings_module(${target_name}
            ${package_option}
            RESOURCES_DIR ${resources_dir}
            SWIG_SOURCES ${swig_sources}
            CPP_SOURCES ${cpp_sources}
            )
    else()
        add_custom_target(${target_name} ALL)
    endif()

#    add_custom_command(TARGET ${target_name} PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory "${resources_dir}"
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${python_sources} "${resources_dir}"
#        )

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
        "PACKAGE"
        "RESOURCES_DIR"
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

    set_lib_install_rules(${target_name} RESOURCE)

    if(ARG_PACKAGE)
        get_target_property(prefix ${target_name} PREFIX)
        set(forward_module "${ARG_RESOURCES_DIR}/${prefix}${target_name}.py")
        file(WRITE ${forward_module} "from ${prefix}${target_name} import *")
    endif()

endfunction()
