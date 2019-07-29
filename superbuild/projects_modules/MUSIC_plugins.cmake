function(MUSIC_plugins_project)

set(ep MUSIC_plugins)

## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies
    medInria
    dtk
    ITK
    VTK
    )


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
## Set up versioning control.
## #############################################################################

#set(git_url ${GITHUB_PREFIX}Inria-Asclepios/music.git)
set(git_url ${GITHUB_PREFIX}fcollot/music.git)
set(git_tag music3)

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

# Set compilation flags
set(${ep}_cxx_flags "${${ep}_cxx_flags} -Wall")
set(${ep}_c_flags "${${ep}_c_flags} -Wall")

set(cmake_args
        ${ep_common_cache_args}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE_externals_projects}
        -DCMAKE_C_FLAGS:STRING=${${ep}_c_flags}
        -DCMAKE_CXX_FLAGS:STRING=${${ep}_cxx_flags}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS_${ep}}
        -DQt5_DIR=${Qt5_DIR}
        -Ddtk_DIR:FILEPATH=${dtk_DIR}
        -DITK_DIR:FILEPATH=${ITK_DIR}
        -DVTK_DIR:FILEPATH=${VTK_DIR}
        -DmedInria_DIR:FILEPATH=${medInria_DIR}
        -DBoost_INCLUDE_DIR=${Boost_INCLUDE_DIR}
        #-DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
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
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS ${cmake_args}
    DEPENDS ${${ep}_dependencies}
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    BUILD_ALWAYS 1
    )

set(EXTERNAL_PROJECT_PLUGINS_LEGACY_DIRS ${EXTERNAL_PROJECT_PLUGINS_LEGACY_DIRS} "${build_path}" PARENT_SCOPE)

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

ExternalProject_Get_Property(${ep} binary_dir)
set(${ep}_DIR ${binary_dir} PARENT_SCOPE)

ExternalProject_Get_Property(${ep} source_dir)
set(${ep}_SOURCE_DIR ${source_dir} PARENT_SCOPE)

endif() #NOT USE_SYSTEM_ep

endfunction()
