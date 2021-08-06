cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

define_property(TARGET PROPERTY PYTHON_MODULES
    BRIEF_DOCS "foo"
    FULL_DOCS "foo")

set(PYTHON_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/python)

include(embed_python)
include(add_python_bindings)
include(add_python_modules)
include(generate_python_resources)
