macro(get_python_binding_library_name var package_name module_name)
    set(var ${package_name}_PYTHON_BINDINGS_${module_name})
endmacro()
