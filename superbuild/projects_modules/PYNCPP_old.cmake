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

set(PYTHON_PACKAGES
    numpy
    CACHE STRING "Third-party Python packages to embed")

function(PYNCPP_project_old)

    set(ep PYNCPP)

    EP_Initialisation(${ep}
        USE_SYSTEM OFF
        BUILD_SHARED_LIBS ON
        REQUIRED_FOR_PLUGINS OFF
        )

    if(NOT USE_SYSTEM_${ep})

        epComputPath(${ep})

        set(project_args
           # GIT_REPOSITORY ${GITHUB_PREFIX}LIRYC-IHU/pyncpp.git
           # GIT_TAG origin/working
            GIT_REPOSITORY ${GITHUB_PREFIX}fcollot/pyncpp.git
            GIT_TAG origin/pack3
            GIT_SHALLOW True
            GIT_PROGRESS True
            )

        set(cmake_args
            -D CMAKE_MACOSX_RPATH:BOOL=OFF
            -D Qt5_DIR:PATH=${Qt5_DIR}
            -D OPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}
            )

        ## #####################################################################
        ## Add external project
        ## #####################################################################

        ExternalProject_Add(${ep}
            PREFIX ${EP_PATH_SOURCE}
            SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
            BINARY_DIR ${build_path}
            TMP_DIR ${tmp_path}
            STAMP_DIR ${stamp_path}
            DEPENDS ${${ep}_dependencies}
            CMAKE_ARGS ${cmake_args}
            INSTALL_COMMAND ""
            "${project_args}"
            )

        ## #####################################################################
        ## Export variables
        ## #####################################################################

        set(${ep}_DIR ${build_path} PARENT_SCOPE)

    endif()

endfunction()
