################################################################################
#
# medInria
#
# Copyright (c) INRIA 2013 - 2018. All rights reserved.
# See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

macro(set_exe_install_rules target)

################################################################################
#
# Usage: set_exe_install_rules(target)
# set rules for the executable designed by the target
#
################################################################################
get_property(GENERATOR_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(${GENERATOR_MULTI_CONFIG})
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/${platformType}Debug/bin)
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/${platformType}Release/bin)
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_BINARY_DIR}/${platformType}MinSizeRel/bin)
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/${platformType}RelWithDebInfo/bin)
else()
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/bin)
endif()

install(TARGETS ${target}
  RUNTIME
  COMPONENT Runtime

  BUNDLE
  DESTINATION bin
  COMPONENT Runtime
  )

## #############################################################################
##  Mac bundle specificites
## #############################################################################

if (APPLE)
  set(MACOSX_BUNDLE_BUNDLE_NAME
    ${target}
    )
  set(MACOSX_BUNDLE_ICON_FILE
    ${target}.icns
    )
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING
    ${${target}_VERSION}
    )
  set(MACOSX_BUNDLE_BUNDLE_VERSION
    ${${target}_VERSION}
    )
  set(MACOSX_BUNDLE_LONG_VERSION_STRING
    "Version ${${target}_VERSION}"
    )
endif()

endmacro()

