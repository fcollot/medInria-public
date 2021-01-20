macro(link_python_bindings_library library_name)

    swig_link_libraries(${library_name} ${ARGN})

endmacro()
