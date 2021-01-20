macro(add_python_package package_name package_directory)

    make_directory(${CMAKE_CURRENT_BINARY_DIR}/python/${package_name})

    file(GLOB_RECURSE ${package_name}_PYTHON_SOURCE_FILES RELATIVE ${package_directory}
        ${package_directory}/*.py
        ${package_directory}/*.py.in
        )

    set(${package_name}_PYTHON_FILES)

    foreach(source_file ${${package_name}_PYTHON_SOURCE_FILES})

        set(configured_file ${source_file})

        if(configured_file MATCHES ".*\.in")
            string(REPLACE ".in" "" configured_file ${configured_file})
        endif()

        configure_file(${package_directory}/${source_file}
            ${CMAKE_CURRENT_BINARY_DIR}/python/${package_name}/${configured_file})

        list(APPEND ${package_name}_PYTHON_FILES ${configured_file})

    endforeach()

    set(${package_name}_PYTHON_BINDINGS_MODULES)

endmacro()
