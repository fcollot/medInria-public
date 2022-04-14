################################################################################
#
# medInria
#
# Copyright (c) INRIA 2022. All rights reserved.
# See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

string(CONCAT USE_PYTHON_DOC
    "Embed Python 3 into the application."
    " Python will be built as an external project and embedded into the application."
    " (If embedded, Python can be deactivated at startup with the --no-python"
    " option)"
    )
option(USE_PYTHON ${USE_PYTHON_DOC} ON)

set(PYTHON_VERSION_MAJOR 3)
set(PYTHON_VERSION_MINOR 10)
set(PYTHON_VERSION_PATCH 4)

function(python_project)

    set(ep python)

## #############################################################################
## List the dependencies of the project
## #############################################################################

#if(${USE_FFmpeg})
#  list(APPEND ${ep}_dependencies ffmpeg)
#endif()

## #############################################################################
## Prepare the project
## #############################################################################

    EP_Initialisation(${ep}
        USE_SYSTEM OFF
        BUILD_SHARED_LIBS ON
        REQUIRED_FOR_PLUGINS ON
        )

    if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Set up versioning control
## #############################################################################

        set(git_url ${GITHUB_PREFIX}python/cpython.git)
        set(git_tag v${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}.${PYTHON_VERSION_PATCH})

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

        # set compilation flags
        if (UNIX)
            set(${ep}_c_flags "${${ep}_c_flags} -w")
            set(${ep}_cxx_flags "${${ep}_cxx_flags} -w")
        endif()

        set(cmake_args
            ${ep_common_cache_args}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE_externals_projects}
            -DCMAKE_C_FLAGS=${${ep}_c_flags}
            -DCMAKE_CXX_FLAGS=${${ep}_cxx_flags}
            -DCMAKE_MACOSX_RPATH:BOOL=OFF
            -DCMAKE_SHARED_LINKER_FLAGS=${${ep}_shared_linker_flags}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS_${ep}}
            -DBUILD_TESTING=OFF
            -DBUILD_DOCUMENTATION=OFF
            -DBUILD_EXAMPLES=OFF
            )

        set(configure_args
            --enable-shared
            --without-static-libpython
            --enable-optimizations
            --disable-test-modules
            )

## #############################################################################
## Add external-project
## #############################################################################

        epComputPath(${ep})

        ExternalProject_Add(${ep}
            PREFIX ${EP_PATH_SOURCE}
            SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
            BINARY_DIR ${build_path}
            TMP_DIR ${tmp_path}
            STAMP_DIR ${stamp_path}
            GIT_REPOSITORY ${git_url}
            GIT_TAG ${git_tag}
            GIT_SHALLOW True
            GIT_PROGRESS True
            DEPENDS ${${ep}_dependencies}
            CONFIGURE_COMMAND ${EP_PATH_SOURCE}/${ep}/configure ${configure_args}
            INSTALL_COMMAND ""
            )

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

        ExternalProject_Get_Property(${ep} binary_dir)
        ExternalProject_Get_Property(${ep} source_dir)
        set(${ep}_BINARY_DIR ${binary_dir} PARENT_SCOPE)
        set(${ep}_SOURCE_DIR ${source_dir} PARENT_SCOPE)

    endif()

endfunction()
