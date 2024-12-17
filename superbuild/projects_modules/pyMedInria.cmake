################################################################################
#
# medInria
#
# Copyright (c) INRIA 2024. All rights reserved.
# See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

function(pyMedInria_project)

    set(ep pyMedInria)

    EP_Initialisation(${ep}
        USE_SYSTEM OFF
        BUILD_SHARED_LIBS ON
        REQUIRED_FOR_PLUGINS ON
        )

    if (NOT USE_SYSTEM_${ep})

        epComputPath(${ep})

        set(download_args
            GIT_REPOSITORY ${GITHUB_PREFIX}FCOLLOT/pyMedInria.git
            GIT_TAG p16
            GIT_SHALLOW True
            GIT_PROGRESS True
            )

        set(cmake_args
            ${ep_common_cache_args}
            -D "CMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE_externals_projects}"
            -D "PYMED_USE_QT5:BOOL=TRUE"
            -D "PYMED_BUILD_PYTHON:BOOL=TRUE"
            -D "PYMED_BUILD_STANDALONE:BOOL=FALSE"
            )

        set(git_url ${GITHUB_PREFIX}medInria/RPI.git)
        set(git_tag RPI_INTERFACE)

        ExternalProject_Add(${ep}
            PREFIX ${EP_PATH_SOURCE}
            SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
            BINARY_DIR ${build_path}
            TMP_DIR ${tmp_path}
            STAMP_DIR ${stamp_path}
            CMAKE_ARGS ${cmake_args}
            INSTALL_COMMAND ""
            "${download_args}"
            )

        set(${ep}_ROOT "${build_path}" PARENT_SCOPE)

    endif() # NOT USE_SYSTEM

endfunction()
