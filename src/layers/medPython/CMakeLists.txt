################################################################################
#
# medInria
#
# Copyright (c) INRIA 2021. All rights reserved.
#
# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt
#
# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

set(TARGET_NAME medPython)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  )

include(python_utils)

find_package(dtk REQUIRED)
include_directories(${dtk_INCLUDE_DIRS})

find_package(Qt5 REQUIRED Core)

find_program(pyinstaller "pyinstaller")

list_source_files(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    )

list_header_directories_to_include(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )

add_library(${TARGET_NAME} SHARED
    ${${TARGET_NAME}_CFILES}
    )

set(pyinstaller_dir "${CMAKE_CURRENT_BINARY_DIR}/pyinstaller")
set(pyinstaller_input "${CMAKE_CURRENT_SOURCE_DIR}/pyinstaller_input.py")

add_custom_target(make_pyinstaller_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${pyinstaller_dir}"
  )

add_custom_target(embed_python
    COMMAND "${pyinstaller}"
    --clean
    --noupx
    --noconfirm
    --name ${TARGET_NAME}
    "${pyinstaller_input}"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_python.cmake"
    COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@executable_path/../../../../lib/Python" lib/Python
    COMMAND ${CMAKE_COMMAND} -E copy_directory lib ${CMAKE_BINARY_DIR}/lib
    WORKING_DIRECTORY ${pyinstaller_dir}
    DEPENDS make_pyinstaller_dir "${pyinstaller_input}"
    )

#add_custom_target(pyinstaller_command ALL
#    DEPENDS "${pyinstaller_input}")

add_dependencies(${TARGET_NAME} embed_python)

#add_external_resources(${TARGET_NAME} Python)

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${${TARGET_NAME}_INCLUDE_DIRS}
    )

#list(POP_FRONT ${Python3_LIBRARIES} PythonLibrary)

#add_library(EmbeddedPython UNKNOWN IMPORTED)
#set_property(TARGET EmbeddedPython PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/libEmbeddedPython.dylib")
#set_property(TARGET EmbeddedPython PROPERTY IMPORTED_IMPLIB "${CMAKE_BINARY_DIR}/lib/libEmbeddedPython.dylib")

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    ${CMAKE_BINARY_DIR}/lib/Python
    medCoreLegacy
    )

set_lib_install_rules(${TARGET_NAME}
    ${${TARGET_NAME}_HEADERS}
    )
