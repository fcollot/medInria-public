macro(add_python_resource resource_name)

    cmake_parse_arguments(resource_name "" "" "PACKAGES" ${ARGN})

    set(qrc_file ${CMAKE_CURRENT_BINARY_DIR}/python/${resource_name}/${resource_name}.qrc)
    file(WRITE ${qrc_file} "<RCC><qresource prefix=\"python\">")

    if (${resource_name}_PACKAGES)
        foreach(package_name ${${resource_name}_PACKAGES})
            foreach(file ${${package_name}_PYTHON_FILES})
                file(APPEND ${qrc_file} <file>${package_name}/${file}</file>)
            endforeach()

            foreach(module_file ${${package_name}_PYTHON_BINDING_MODULES})
                file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/python/${package_name}/${module_file})
                file(APPEND ${qrc_file} <file>${package_name}/${module_file}</file>)
            endforeach()
        endforeach()
    endif()

    file(WRITE ${qrc_file} "<qresource></RCC>")

    qt5_add_binary_resources(${resource_name}
        ${qrc_file}
        DESTINATION ${CMAKE_BINARY_DIR}/resources/${resource_name}.rcc
        )

    if (${resource_name}_PACKAGES)
        foreach(package_name ${${resource_name}_PACKAGES})
            foreach(binding_library ${package_name}_PYTHON_BINDING_LIBRARIES)
                add_dependencies(${resource_name} ${binding_library}_swig_compilation)
            endforeach()
        endforeach()
    endif()

endmacro()
