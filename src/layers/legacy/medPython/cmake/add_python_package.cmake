macro(add_python_package package_name)

    cmake_parse_arguments(${package_name}_PYTHON "" "SOURCE_DIRECTORY" "BINDINGS" ${ARGN})

    set(${package_name}_PYTHON_SOURCE_DIRECTORY ${source_directory})

    file(GLOB_RECURSE ${package_name}_PYTHON_SOURCE_FILES RELATIVE ${${package_name}_PYTHON_SOURCE_DIRECTORY}
        ${${package_name}_SOURCE_DIRECTORY}/*.py
        ${${package_name}_SOURCE_DIRECTORY}/*.py.in
        )

    set(${package_name}_PYTHON_FILES)

    foreach(source_file ${${package_name}_PYTHON_SOURCE_FILES})
        set(configured_file ${source_file})

        if(configured_file MATCHES ".*\.in")
            string(REPLACE ".in" "" configured_file ${configured_file})
        endif()

        configure_file(${${package_name}_PYTHON_SOURCE_DIRECTORY}/${source_file}
            ${CMAKE_CURRENT_BINARY_DIR}/python/${package_name}/${configured_file})
        list(APPEND ${package_name}_PYTHON_FILES ${configured_file})
    endforeach()

    set(${package_name}_PYTHON_BINDING_MODULES)
    set(${package_name}_PYTHON_BINDING_LIBRARIES)

    if(${package_name}_PYTHON_BINDINGS)
        foreach(bindings_file ${${package_name}_BINDINGS})
            get_filename_component(bindings_module ${bindings_file} NAME_WE)
            set(configured_bindings_file ${CMAKE_CURRENT_BINARY_DIR}/bindings/${package_name}/${bindings_module}.i)
            configure_file(${bindings_file} ${configured_bindings_file})
            set_source_files_properties(${configured_bindings_file}
                PROPERTIES CPLUSPLUS ON
                USE_TARGET_INCLUDE_DIRECTORIES TRUE
                )
            list(APPEND ${package_name}_PYTHON_BINDING_MODULES ${bindings_module}.py)

            get_python_binding_library_name(bindings_library_name ${package_name} ${bindings_module})
            list(APPEND ${package_name}_PYTHON_BINDING_LIBRARIES ${bindings_library_name})

            swig_add_library(${bindings_library_name}
                TYPE MODULE
                LANGUAGE python
                OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/python/${package_name}
                SOURCES
                ${configured_bindings_file}
                )

            swig_link_libraries(${bindings_library_name}
                ${Python3_LIBRARIES}
                )

            set_lib_install_rules(${bindings_library_name})
        endforeach()
    endif()

endmacro()
