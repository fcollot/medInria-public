find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(Python3 COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})

include(add_python_package)
include(add_python_bindings_module)
include(add_python_resource)
include(link_python_bindings_library)

set(SWIG_RUNTIME_HEADER_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/swig)
set(SWIG_RUNTIME_HEADER ${SWIG_RUNTIME_HEADER_DIRECTORY}/swig_runtime.h)

add_custom_command(
    OUTPUT ${SWIG_RUNTIME_HEADER}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SWIG_RUNTIME_HEADER_DIRECTORY}
    COMMAND ${SWIG_EXECUTABLE}
    ARGS
    "-python"
    "-c++"
    "-external-runtime"
    ${SWIG_RUNTIME_HEADER}
    )
