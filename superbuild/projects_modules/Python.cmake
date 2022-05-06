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

function(python_project)

    set(ep Python)

    set(version_major 3)
    set(version_minor 10)
    set(version_patch 4)

    EP_Initialisation(${ep}
        USE_SYSTEM OFF
        BUILD_SHARED_LIBS ON
        REQUIRED_FOR_PLUGINS OFF
        )

    find_package(SWIG REQUIRED COMPONENTS python)

    if(USE_SYSTEM_${ep})
        find_package(Python ${version_major}.${version_minor}.${version_patch}
            REQUIRED COMPONENTS Interpreter Development
            )
    else()
        epComputPath(${ep})

        set(source_dir ${EP_PATH_SOURCE}/${ep})
        set(binary_dir ${build_path})

        if(WIN32)
            ## #################################################################
            ## (Windows) Download an embeddable Python package
            ## #################################################################

            set(project_args
                URL "https://med.inria.fr/downloads/embedded-python/python-${version_major}.${version_minor}.${version_patch}-win.zip"
                CONFIGURE_COMMAND ""
                BUILD_COMMAND ""
                )

        else()
            ## #################################################################
            ## (Unix) Compile a customized Python
            ## #################################################################

            set(configure_args
                --enable-shared
                --without-static-libpython
                --enable-optimizations
                --disable-test-modules
                )

            set(project_args
                GIT_REPOSITORY ${GITHUB_PREFIX}fcollot/cpython.git
                GIT_TAG v${version_major}.${version_minor}.${version_patch}_with_pth
                GIT_SHALLOW True
                GIT_PROGRESS True
                CONFIGURE_COMMAND ${source_dir}/configure ${configure_args}
                )

        endif()

        ## #####################################################################
        ## Add external project
        ## #####################################################################

        ExternalProject_Add(${ep}
            PREFIX ${EP_PATH_SOURCE}
            SOURCE_DIR ${source_dir}
            BINARY_DIR ${binary_dir}
            TMP_DIR ${tmp_path}
            STAMP_DIR ${stamp_path}
            DEPENDS ${${ep}_dependencies}
            INSTALL_COMMAND ""
            "${project_args}"
            )

        ExternalProject_Get_Property(${ep} source_dir)
        ExternalProject_Get_Property(${ep} binary_dir)

        ## #####################################################################
        ## Export variables
        ## #####################################################################

        set(${ep}_DIR ${source_dir} PARENT_SCOPE)
        set(${ep}_VERSION "${version_major}.${version_minor}.${version_patch}" PARENT_SCOPE)
        set(SWIG_EXECUTABLE ${SWIG_EXECUTABLE} PARENT_SCOPE)
        set(${PROJECT_NAME}_RESOURCE_DIRS ${${PROJECT_NAME}_RESOURCE_DIRS} ${Python_STDLIB})

        ## #####################################################################
        ## Create config files
        ## #####################################################################

        include(CMakePackageConfigHelpers)

        file(RELATIVE_PATH relative_source_dir ${EP_PREFIX} ${source_dir})

        configure_package_config_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/PythonConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/PythonConfig.cmake"
            INSTALL_PREFIX ${EP_PREFIX}
            INSTALL_DESTINATION ${relative_source_dir}
            PATH_VARS source_dir binary_dir
            )

        write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/PythonConfigVersion.cmake"
            VERSION ${version_major}.${version_minor}.${version_patch}
            COMPATIBILITY SameMinorVersion
            )

        ExternalProject_Add_Step(${ep} copy_config_files
            COMMAND ${CMAKE_COMMAND} -E copy_if_different PythonConfig.cmake PythonConfigVersion.cmake "<SOURCE_DIR>"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDEES build
            )

    endif()

endfunction()
