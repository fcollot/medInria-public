################################################################################
#
# medInria
#
# Copyright (c) INRIA 2021. All rights reserved.
# See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

function(set_external_resources target)

################################################################################
#
# Usage: set_external_resources(target
#                               [DESTINATION destination]
#                               [FILES [resources...]]
#                               [DIRECTORIES [directories...]])
#
# Adds a target with the given name that copies the resources to the resources
# folder of the build directory, and configures the install process so that the
# resources are correctly imported during packaging.
#
# The options are:
#
# ALL
#     Indicate that this target should be added to the default build target so
#     so that it will be run every time (see 'add_custom_target').
#
# DESTINATION
#     The resources will be placed in the specified subdirectory of the
#     resources folder.
#
# RESOURCES
#     Specify resources files or directories to add (the contents of the
#     directories will be copied to the destination folder). File-level
#     dependencies will be set up for individually specified files only.
#
################################################################################

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        "APPEND"
        "DESTINATION"
        "FILES;DIRECTORIES"
        )

    if (NOT (ARG_FILES OR ARG_DIRECTORIES))
        message(FATAL_ERROR "No resources specified.")
    endif()

    if (ARG_DESTINATION)
        set(destination "${ARG_DESTINATION}")
    else()
        set(destination ".")
    endif()

    if (ARG_APPEND)
        set(append APPEND)
    else()
        set(append)
    endif()

    _build_resource_group(resources ${destination} FILES ${ARG_FILES} DIRECTORIES ${ARG_DIRECTORIES})
    set_property(TARGET ${target} ${append} PROPERTY ${PROJECT_NAME}_RESOURCES ${resources})

endfunction()

function(import_external_resources target)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "DESTINATION"
        "FROM_TARGETS;FILES;DIRECTORIES"
        )

    get_target_property(target_type ${target} TYPE)

    if ((NOT target_type STREQUAL "EXECUTABLE")
        AND
        (NOT target_type STREQUAL "SHARED_LIBRARY"))
        message(FATAL_ERROR "Target must be an executable or a shared library.")
    endif()

    if (ARG_DESTINATION)
        set(base_destination "${ARG_DESTINATION}")
    else()
        set(base_destination ".")
    endif()

    _get_resources_parent_directory(target_dir ${target})
    _get_resources_directory(resources_base_dir ${target} ${target_type})
    set(resources_base_dir ${resources_base_dir}$<$<NOT:$<STREQUAL:${base_destination},".">>:/${base_destination}>)
    _convert_to_bundle_if_needed(${target} ${target_type})

    if (ARG_FILES OR ARG_DIRECTORIES)
        set_external_resources(${target} FILES ${ARG_FILES} DIRECTORIES ${ARG_DIRECTORIES})
        _import_resources(${target} ${target_dir} ${resources_base_dir} FILES ${ARG_FILES} DIRECTORIES ${ARG_DIRECTORIES})
    endif()

    if (ARG_FROM_TARGETS)
        foreach(source_target ${ARG_FROM_TARGETS})
            get_target_property(resources ${source_target} ${PROJECT_NAME}_RESOURCES)
            while (resources)
                _pop_resource_group(resources destination files directories)
                set(resources_dir ${resources_base_dir}$<$<NOT:$<STREQUAL:${destination},".">>:/${destination}>)
                _import_resources(${target} ${target_dir} ${resources_dir} FILES ${files} DIRECTORIES ${directories})
            endwhile()

            get_target_property(sub_targets ${source_target} ${PROJECT_NAME}_PYTHON_PACKAGE_SUB_TARGETS)
            if (sub_targets)
                import_external_resources(${target} DESTINATION ${base_destination} FROM_TARGETS ${sub_targets})
            endif()
        endforeach()

        add_dependencies(${target} ${ARG_FROM_TARGETS})
    endif()

endfunction()

function(_import_resources target base_dir sub_dir)

    cmake_parse_arguments(PARSE_ARGV 3 "ARG"
        ""
        ""
        "FILES;DIRECTORIES"
        )

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${sub_dir}"
        WORKING_DIRECTORY "${base_dir}"
        )

    if (ARG_FILES)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ARG_FILES} "${sub_dir}"
            WORKING_DIRECTORY "${base_dir}"
            )
        if (NOT APPLE)
            install(FILES ${ARG_FILES} DESTINATION "${sub_dir}")
        endif()
    endif()

    if (ARG_DIRECTORIES)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_DIRECTORIES} "${sub_dir}"
            WORKING_DIRECTORY "${base_dir}"
            )
        if (NOT APPLE)
            set(adjusted_directories)
            foreach (directory ${ARG_DIRECTORIES})
                list(APPEND adjusted_directories ${directory}/)
            endforeach()
            install(DIRECTORY ${adjusted_directories} DESTINATION "${sub_dir}")
        endif()
    endif()

endfunction()



macro(_build_resource_group resources_var destination)

    cmake_parse_arguments("ARG"
        ""
        ""
        "FILES;DIRECTORIES"
        ${ARGN}
        )

    set(${resources_var} ${destination} _FILES_ ${ARG_FILES} _DIRECTORIES_ ${ARG_DIRECTORIES} _END_GROUP_)

endmacro()

macro(_pop_resource_group resources_var destination_var files_var directories_var)

    list(POP_FRONT ${resources_var} ${destination_var} item)
    set(${files_var})
    set(${directories_var})
    set(current_var)

    while (NOT item STREQUAL "_END_GROUP_")
        if (item STREQUAL _FILES_)
            set(current_var ${files_var})
        elseif (item STREQUAL _DIRECTORIES_)
            set(current_var ${directories_var})
        else()
            list(APPEND ${current_var} ${item})
        endif()
        list(POP_FRONT ${resources_var} item)
    endwhile()

endmacro()

macro(_get_resources_parent_directory parent_dir_var target)

    set(${parent_dir_var} $<IF:$<PLATFORM_ID:Darwin>,$<TARGET_BUNDLE_CONTENT_DIR:${target}>,${CMAKE_BINARY_DIR}>)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (${is_multi_config})
        set(config_subdir /$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>$<$<CONFIG:MinSizeRel>:MinSizeRel>)
        string(APPEND ${parent_dir_var} $<$<NOT:$<PLATFORM_ID:Darwin>>:/${config_subdir}>)
    endif()

endmacro()

macro(_get_resources_directory resources_dir_var target target_type)

    set(${resources_dir_var} $<IF:$<PLATFORM_ID:Darwin>,Resources,resources>)
    if (${target_type} STREQUAL "SHARED_LIBRARY")
        string(APPEND ${resources_dir_var} $<$<NOT:$<PLATFORM_ID:Darwin>>:/${target}>)
    endif()

endmacro()

function(_convert_to_bundle_if_needed target target_type)

    if (APPLE)
        if (target_type STREQUAL "SHARED_LIBRARY")
            get_target_property(is_framework ${target} FRAMEWORK)
            if (NOT is_framework)
                set_target_properties(${target} PROPERTIES
                    FRAMEWORK TRUE
                    MACOSX_BUNDLE_BUNDLE_NAME ${target}
                    MACOSX_FRAMEWORK_IDENTIFIER ${${PROJECT_NAME}_IDENTIFIER}.${target}
                    )
            endif()
        endif()
    endif()

endfunction()
