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

function(medInria_python_project)

    set(ep medInria_python)

    EP_Initialisation(${ep}
        USE_SYSTEM OFF
        BUILD_SHARED_LIBS ON
        REQUIRED_FOR_PLUGINS OFF
        )

    if(NOT USE_SYSTEM_${ep})

        ## #####################################################################
        ## Configuration
        ## #####################################################################

        set(project_args
            PREFIX "${CMAKE_BINARY_DIR}/medInria_python-build"
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/python"
            BINARY_DIR "${CMAKE_BINARY_DIR}/medInria_python-build"
            )

        set(cmake_args
          ${ep_common_cache_args}
          -D "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE_externals_projects}"
          -D "CMAKE_C_FLAGS=${${ep}_c_flags}"
          -D "CMAKE_CXX_FLAGS=${${ep}_cxx_flags}"
          -D "CMAKE_MACOSX_RPATH:BOOL=OFF"
          -D "CMAKE_SHARED_LINKER_FLAGS=${${ep}_shared_linker_flags}"
          -D "CMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
          -D "BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS_${ep}}"
          -D "FETCHCONTENT_BASE_DIR:PATH=<BINARY_DIR>"
          -D "medInria_python_VERSION:STRING=${${PROJECT_NAME}_VERSION}"
          -D "GITHUB_PREFIX:STRING=${GITHUB_PREFIX}"
          -D "Qt5_DIR:PATH=${Qt5_DIR}"
          -D "OPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}"
          )

        ## #####################################################################
        ## Add project
        ## #####################################################################

        ExternalProject_Add(${ep}
            ${project_args}
            CMAKE_ARGS ${cmake_args}
            CMAKE_GENERATOR ${gen}
            CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
            UPDATE_COMMAND ""
            INSTALL_COMMAND ""
            )

        ## #####################################################################
        ## Export variables
        ## #####################################################################

        ExternalProject_Get_Property(${ep} binary_dir)
        set(${ep}_DIR ${binary_dir} PARENT_SCOPE)

    endif()

endfunction()
