macro(link_python_binding_library library_name)

    swig_link_libraries(${library_name} ${ARGN})

endmacro()
