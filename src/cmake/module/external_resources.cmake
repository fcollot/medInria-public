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



function(add_external_resources target_name)

################################################################################
#
# Usage: add_external_resources(target
#                               [DESTINATION destination]
#                               [FILES [files...]])
#
# Adds post-build commands to 'target' that copy external resource files to the
# project resource directory.
# The options are:
#
# DESTINATION
#     (optional) Specify the subdirectory (relative to the resource directory)
#     into which the resources will be copied.
#
# FILES
#     Specify the files to copy.
#
################################################################################

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "DESTINATION"
        "FILES"
        )

    get_external_resources_build_directory(resources_dir)

    if (ARG_DESTINATION)
        set(resources_dir "${resources_dir}/${ARG_DESTINATION}")
    endif()

    if (ARG_FILES)
        add_custom_target(${target_name} ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory "${resources_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ARG_FILES} "${resources_dir}"
        )
#    if (NOT APPLE)
#        install(FILES ${ARG_FILES} DESTINATION "${sub_dir}")
    endif()

endfunction()
