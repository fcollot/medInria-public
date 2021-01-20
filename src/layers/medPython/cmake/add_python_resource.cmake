macro(add_python_resource resource_name)

    cmake_parse_arguments(${resource_name} "" "" "PACKAGES" ${ARGN})

    set(qrc_file ${CMAKE_CURRENT_BINARY_DIR}/python/${resource_name}.qrc)
    file(WRITE ${qrc_file} "<RCC><qresource prefix=\"/python\">")

    foreach(package_name ${${resource_name}_PACKAGES})
        foreach(file ${${package_name}_PYTHON_FILES})
            file(APPEND ${qrc_file} <file>${package_name}/${file}</file>)
        endforeach()
    endforeach()

    foreach(file ${${resource_name}_UNPARSED_ARGUMENTS})
        file(APPEND ${qrc_file} <file>${file}</file>)
    endforeach()

    file(APPEND ${qrc_file} "</qresource></RCC>")

    qt5_add_binary_resources(${resource_name}
        ${qrc_file}
        DESTINATION ${CMAKE_BINARY_DIR}/resources/${resource_name}.rcc
        )

    foreach(package_name ${${resource_name}_PACKAGES})
        foreach(binding_module ${${package_name}_PYTHON_BINDINGS_MODULES})
            add_dependencies(${resource_name} ${binding_module}_swig_compilation)
        endforeach()
    endforeach()

endmacro()
