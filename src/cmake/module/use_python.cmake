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

#string(CONCAT EMBED_Python_DOC
#    "Embed Python into the application."
#    " On Unix this option requires that the Python external project was added to"
#    " the superbuild (USE_SYSTEM_Python OFF)."
#    )
#option(EMBED_Python ${EMBED_Python_DOC} ON)

## #############################################################################
## Find SWIG
## #############################################################################

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(SWIG REQUIRED COMPONENTS python)
include(${SWIG_USE_FILE})

## #############################################################################
## Find Python
## #############################################################################

find_package(Python ${Python_VERSION}
    REQUIRED COMPONENTS Interpreter Development
    )

## #############################################################################
## Add core Python library as imported target
## #############################################################################

add_library(Python_LIBRARY SHARED IMPORTED)

target_include_directories(Python_LIBRARY INTERFACE ${Python_INCLUDE_DIRS})

if(WIN32)
    foreach(_library ${Python_LIBRARIES})
        get_filename_component(_libext "${_library}" LAST_EXT)
        get_filename_component(_libname "${_library}" NAME_WLE)

        if(_libext STREQUAL ".dll")
            if(_libname MATCHES ".*_d.")
                set(_runtime_lib_debug "${_library}")
            else()
                set(_runtime_lib "${_library}")
            endif()
        elseif(_libex STREQUAL ".lib")
            if(_libname MATCHES ".*_d.")
                set(_stub_lib_debug "${_library}")
            else()
                set(_stub_lib "${_library}")
            endif()
        endif()
    endforeach()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(_location "${_runtime_lib_debug}")
        set(_implib "${_stub_lib_debug}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_location "${_runtime_lib}")
        set(_implib "${_stub_lib}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(_location "${_runtime_lib_debug}")
        set(_implib "${_stub_lib_debug}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(_location "${_runtime_lib}")
        set(_implib "${_stub_lib}")
    endif()

    set_target_properties(Python_LIBRARY PROPERTIES
        IMPORTED_LOCATION_DEBUG "${_runtime_lib_debug}"
        IMPORTED_IMPLIB_DEBUG "${_stub_lib_debug}"
        IMPORTED_LOCATION_RELEASE "${_runtime_lib}"
        IMPORTED_IMPLIB_RELEASE "${_stub_lib}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${_runtime_lib_debug}"
        IMPORTED_IMPLIB_RELWITHDEBINFO "${_stub_lib_debug}"
        IMPORTED_LOCATION_MINSIZEREL "${_runtime_lib}"
        IMPORTED_IMPLIB_MINSIZEREL "${_stub_lib}"
        IMPORTED_LOCATION "${_location}"
        IMPORTED_IMPLIB "${_implib}"
        )
else()
    set_target_properties(Python_LIBRARY PROPERTIES
        IMPORTED_LOCATION "${Python_LIBRARIES}"
        IMPORTED_SONAME "${Python_LIBRARIES}"
        )
endif()

## #############################################################################
## Fix STDARCH path
## #############################################################################

# (CMake's FindPython doesn't give the correct STDARCH path)

if(WIN32)
    set(libext ".dll")
else()
    set(libext ".so")
endif()

file(GLOB stdarch_libs "${Python_STDARCH}/*${libext}")

if(NOT stdarch_libs)
    if(WIN32)
        set(Python_STDARCH "${Python_STDLIB}/../DLLs")
    else()
        set(Python_STDARCH "${Python_STDLIB}/lib-dynload")
    endif()

    file(GLOB stdarch_libs "${Python_STDARCH}/*${libext}")

    if(NOT stdarch_libs)
        message(FATAL_ERROR "Cannot find the Python_STDARCH libraries.")
    endif()
endif()

## #############################################################################
## Compile definitions
## #############################################################################

add_compile_definitions(USE_PYTHON)

## #############################################################################
## Embedding function
## #############################################################################

function(embed_python target)

    if(NOT (WIN32 OR Python_WITH_PTH_SUPPORT))
        string(CONCAT message_text
            "This version of Python does not support path configuration using a"
            " _pth file, which is required for embedding. The option is officially"
            " supported on Windows, but on MacOS and Linux only the customized"
            " external project provides it. Make sure the superbuild is configured"
            " with USE_SYSTEM_Python OFF (to build the external project) or else"
            " configure medInria with EMBED_Python OFF to disable embedding."
            )
        message(FATAL_ERROR ${message_text})
    else()
        ## #####################################################################
        ## Embed executable
        ## #####################################################################

        if (WIN32)
            set(copied_executable "python.exe")
        else()
            set(copied_executable "python")
        endif()

        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Python_EXECUTABLE}" ${copied_executable}
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:${target}>"
            )

        ## #########################################################################
        ## Embed core library
        ## #########################################################################

#        set(core_library_name libpython${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}.dylib)
#        set(core_library "${Python_BUILD_DIR}/${core_library_name}")

#        if (APPLE)
#            set(copied_core_library "$<TARGET_BUNDLE_CONTENT_DIR:${target}>/Frameworks/${core_library_name}")
#        else()
#            set(copied_core_library "lib/${core_library_name}")
#            get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

#            if (${is_multi_config})
#                string(PREPEND copied_core_library "$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>$<$<CONFIG:MinSizeRel>:MinSizeRel>/")
#            endif()

#            string(PREPEND copied_core_library ${CMAKE_BINARY_DIR}>)
#        endif()

#        add_custom_command(TARGET ${target} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${core_library}" "${copied_core_library}"
#            )

#        if (APPLE)
#            add_custom_command(TARGET ${target} POST_BUILD
#                COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "${copied_core_library}" "${copied_core_library}"
#                )
#        endif()



        set(Python_EXCLUDED_MODULES
            test
            turtledemo
            tkinter
            )
    endif()

endfunction()
