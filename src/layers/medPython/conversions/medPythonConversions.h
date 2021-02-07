#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonPrimitiveConversions.h"
#include "medPythonQHashConversions.h"
#include "medPythonQListConversions.h"
#include "medPythonQStringConversions.h"
#include "medPythonQVariantConversions.h"

// Conversions between C++ and Python are defined by overloading the
// medPython_convert function (this function is in the global namespace in order
// to facilitate argument-dependant lookup):
//
// The overload to convert a C++ type to Python has the following signature:
//
//     MEDPYTHON_EXPORT bool medPython_convert(INPUT_TYPE value, PyObject** output);
//
// The output must be a new reference to a python object (meaning the caller
// owns it). The function must return true if the conversion/ succeeded, false
// otherwise. If the conversion failed a Python error must be raised (without
// throwing a C++ exception). If the input type is a pointer, the caller retains
// ownership of the input object.
//
// The overload to convert a Python object to C++ has the following signature:
//
//     MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, OUTPUT_TYPE* output);
//
// The caller retains ownership of the input object and, if the output type is a
// pointer, the caller owns the allocated output object. The function must
// return true if the conversion succeeded, false otherwise. If the conversion
// failed a Python error must be raised (without throwing a C++ exception).
