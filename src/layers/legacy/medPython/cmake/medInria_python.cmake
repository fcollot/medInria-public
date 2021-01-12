find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(Python3 COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})

include(add_python_package)
include(add_python_resource)
include(link_python_binding_library)
include(get_python_binding_library_name)
