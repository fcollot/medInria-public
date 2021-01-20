macro(add_python_bindings_module module_name)

    cmake_parse_arguments(${module_name}
        ""
        "PACKAGE;REMOVE_BINDINGS_PREFIX"
        "SIMPLE_BINDINGS;DIRECTOR_BINDINGS;INCLUDE_INTERFACES;IMPORT_INTERFACES;INCLUDE_HEADERS"
        ${ARGN})

    if(DEFINED ${module_name}_PACKAGE)
        set(module_path ${${module_name}_PACKAGE}/${module_name})
        file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/python/${module_path}.py)
        list(APPEND ${${module_name}_PACKAGE}_PYTHON_FILES ${module_name}.py)
        list(APPEND ${${module_name}_PACKAGE}_PYTHON_BINDINGS_MODULES ${module_name})
    else()
        set(module_path ${module_name})
    endif()

    set(${module_name}_INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/bindings/${module_path}.i)

    string(CONCAT interface_content
        "%include \"standard_defs.i\"\n\n"
        "%module(package=\"${${module_name}_PACKAGE}\", directors=\"1\") ${module_name}\n\n"
        "%feature(\"director:except\")\n"
        "{\n"
        "  if ($error != nullptr)\n"
        "  {\n"
        "    throw Swig::DirectorMethodException()\;\n"
        "  }\n"
        "}\n\n"
        )

    file(WRITE ${${module_name}_INTERFACE} ${interface_content})

    foreach(file ${${module_name}_INCLUDE_HEADERS})
        set(interface_content "%{\n#include \"${file}\"\n%}\n\n")
        file(APPEND ${${module_name}_INTERFACE} ${interface_content})
    endforeach()

    foreach(file ${${module_name}_IMPORT_INTERFACES})
        set(interface_content "%import \"${file}\"\n\n")
        file(APPEND ${${module_name}_INTERFACE} ${interface_content})
    endforeach()

    foreach(binding ${${module_name}_DIRECTOR_BINDINGS})
        set(interface_content "%feature(\"director\") ${binding}\;\n\n")
        file(APPEND ${${module_name}_INTERFACE} ${interface_content})
    endforeach()

    foreach(binding ${${module_name}_SIMPLE_BINDINGS} ${${module_name}_DIRECTOR_BINDINGS})
        if (DEFINED ${module_name}_REMOVE_BINDINGS_PREFIX)
            STRING(REGEX REPLACE "^${${module_name}_REMOVE_BINDINGS_PREFIX}" "" unprefixed_binding ${binding})
            set(interface_content "%rename(${unprefixed_binding}) ${binding}\;\n\n")
        endif()

        file(APPEND ${${module_name}_INTERFACE} ${interface_content})

        string(CONCAT interface_content
            "%{\n"
            "#include \"${binding}.h\"\n"
            "%}\n\n"
            "%include \"${binding}.h\"\n\n"
            )

        file(APPEND ${${module_name}_INTERFACE} ${interface_content})
    endforeach()

    foreach(file ${${module_name}_INCLUDE_INTERFACES})
        set(interface_content "%include \"${file}\"\n\n")
        file(APPEND ${${module_name}_INTERFACE} ${interface_content})
    endforeach()

    set_source_files_properties(${${module_name}_INTERFACE}
        PROPERTIES CPLUSPLUS ON
        USE_TARGET_INCLUDE_DIRECTORIES TRUE
        )

    swig_add_library(${module_name}
        TYPE MODULE
        LANGUAGE python
        OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/python/${${module_name}_PACKAGE}
        SOURCES
        ${${module_name}_INTERFACE}
        )

    set_property(TARGET ${module_name} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES 1)

    swig_link_libraries(${module_name}
        ${Python3_LIBRARIES}
        )

endmacro()
