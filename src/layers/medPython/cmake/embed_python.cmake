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
###############################################################################

file(GLOB_RECURSE libraries
    "Python"
    "*.so"
    "*.dylib"
    "*.framework"
    "*.dll")

file(COPY ${libraries} DESTINATION lib)
#file(RENAME lib/Python lib/libEmbeddedPython.dylib)
