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

set(CPACK_BINARY_TGZ OFF)
set(CPACK_BINARY_DRAGNDROP ON)
set(CPACK_BINARY_PACKAGEMAKER OFF)
set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_LIST_DIR}/medInria-logo-dmg.png")
set(CPACK_PACKAGE_EXECUTABLES medInria)

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(MACDEPLOYQT macdeployqt HINTS "${_qt_bin_dir}")
find_program(DTKDEPLOY dtkDeploy HINTS "${dtk_DIR}/bin")

set(CPACK_INSTALL_CMAKE_PROJECTS
    ${medInria_DIR} medInria Runtime "/"
    ${PYNCPP_DIR} pyncpp_superbuild ALL "/"
 #   ${medInria_python_DIR} medInria_python Runtime "/"
    ${PROJECT_BINARY_DIR} ${PROJECT_NAME} ALL "/"
    )

set(plugin_dirs
    "${medInria_BINARY_DIR}/bin/plugins"
    "${medInria_BINARY_DIR}/bin/plugins_legacy"
    ${PRIVATE_PLUGINS_DIRS}
    ${PRIVATE_PLUGINS_LEGACY_DIRS}
    )

set(inject_dirs)

foreach(plugin_dir ${plugin_dirs})
    string(APPEND inject_dirs " -inject-dir=${plugin_dir}")
endforeach()

install(CODE "
    # Move application to root folder of dmg
#    file(RENAME \"\${CMAKE_INSTALL_PREFIX}/bin/medInria.app\" \"\${CMAKE_INSTALL_PREFIX}/medInria.app\")
#    file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/bin\")

#    execute_process(
#        COMMAND ${MACDEPLOYQT} \"\${CMAKE_INSTALL_PREFIX}/medInria.app\" -appstore-compliant
#        COMMAND ${DTKDEPLOY} \"\${CMAKE_INSTALL_PREFIX}/medInria.app\" ${inject_dirs}
#        COMMAND_ERROR_IS_FATAL ANY
#        )

#    include(BundleUtilities)
#    set(BU_CHMOD_BUNDLE_ITEMS TRUE)
#    set(dirs \"${medInria_BINARY_DIR}/lib\" ${PYNCPP_LIBRARY_DIRS})
#    fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/medInria.app\" \"\" \"\${dirs}\")
    "
    )
