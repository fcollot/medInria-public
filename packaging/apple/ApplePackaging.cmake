##############################################################################
#
# medInria
#
# Copyright (c) INRIA 2013. All rights reserved.
# See LICENSE.txt for details.
# 
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

set(CPACK_BINARY_TGZ ON)

set(CPACK_BINARY_DRAGNDROP OFF)
set(CPACK_BINARY_PACKAGEMAKER OFF)

#set(CPACK_DMG_DS_STORE "${CMAKE_CURRENT_LIST_DIR}/ds_store")

set(CPACK_DMG_BACKGROUND_IMAGE
    "/Users/florent.collot/PaPaPou_girl_riding_horse_through_dystopian_Tokyo_in_the_style__bf70e5b7-8954-474a-aa22-2c8a360a4314.png"
    )

## #############################################################################
## Get distribution name and architecture
## #############################################################################

set(CPACK_PACKAGE_FILE_NAME 
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}"
   )


## #############################################################################
## Add Apple packaging script
## #############################################################################

configure_file(${PROJECT_SOURCE_DIR}/apple/ApplePackScript.cmake.in
  ${PROJECT_BINARY_DIR}/tmp.in
  )
  
configure_file(
  ${PROJECT_BINARY_DIR}/tmp.in 
  ${PROJECT_BINARY_DIR}/apple/ApplePackScript.cmake
  )
  
configure_file(${PROJECT_SOURCE_DIR}/apple/mac_packager.sh.in
  ${PROJECT_BINARY_DIR}/apple/mac_packager.sh
  @ONLY
  )

#set(CPACK_INSTALL_SCRIPT
#  ${PROJECT_BINARY_DIR}/apple/ApplePackScript.cmake
#  )

#foreach(external_project medInria;pyncpp)
#    if(DEFINED ${external_project}_ROOT)
#        install(CODE "
#            if(EXISTS \"${${external_project}_ROOT}/CMakeCache.txt\")
#                message(\"Installing ${external_project}...\")
#                execute_process(
#                    COMMAND ${CMAKE_COMMAND}
#                    --install ${${external_project}_ROOT}
#                    --prefix \"\${CMAKE_INSTALL_PREFIX}\"
#                    --component Runtime
#                    )
#                file(SIZE \"${${external_project}_ROOT}/install_manifest_Runtime.txt\" runtime_manifest_size)
#                if(runtime_manifest_size EQUAL 0)
#                    message(\"${external_project} has no Runtime component. Falling back to default install.\")
#                    execute_process(
#                        COMMAND ${CMAKE_COMMAND}
#                        --install ${${external_project}_ROOT}
#                        --prefix \"\${CMAKE_INSTALL_PREFIX}\"
#                        )
#                endif()
#            endif()
#            ")
#    endif()
#endforeach()

find_package(Qt5 REQUIRED COMPONENTS Core)
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
set(qt_plugins_dir "${_qt_bin_dir}/../plugins")

set(qt_plugins_to_install
    imageformats
    styles
    sqldrivers
    platforms
    printsupport
    )

set(tmp_dir "\${CMAKE_INSTALL_PREFIX}/tmp")
set(bundle_path "\${CMAKE_INSTALL_PREFIX}/${CPACK_PACKAGE_NAME}.app")

install(CODE "
    execute_process(COMMAND ${CMAKE_COMMAND}
        --install ${medInria_ROOT}
        --prefix \"${tmp_dir}\"
        )

    execute_process(COMMAND ${CMAKE_COMMAND}
        --install ${pyncpp_ROOT}
        --prefix \"${tmp_dir}\"
        --component Runtime
        )

    file(RENAME \"${tmp_dir}/bin/${CPACK_PACKAGE_NAME}.app\" \"${bundle_path}\")



#    file(REMOVE \"${tmp_dir}\")

    foreach(qt_plugin ${qt_plugins_to_install})
        file(GLOB plugin_files \"${qt_plugins_dir}/\${qt_plugin}/*.dylib\")
        file(INSTALL \${plugin_files} DESTINATION \"${bundle_path}/Contents/Plugins/\${qt_plugin}\")
    endforeach()

    include(BundleUtilities)
    set(BU_CHMOD_BUNDLE_ITEMS TRUE)



#    fixup_bundle(\"${bundle_path}\"
#        \"\${bundled_plugins}\"
#        \"${bundle_path}/Contents/Frameworks;${bundle_path}/Contents/Plugins\"
#        IGNORE_ITEM \"${bundle_path}/Contents/MacOS/${CPACK_PACKAGE_NAME}\"
#        )
    "
    )
