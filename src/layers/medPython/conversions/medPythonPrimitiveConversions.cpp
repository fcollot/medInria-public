/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonPrimitiveConversions.h"

#include "medPythonCore.h"

using namespace med::python;

bool medPython_convert(bool value, PyObject** output)
{
    *output = PyBool_FromLong(value);
    return *output;
}

bool medPython_convert(const PyObject* object, bool* output)
{
    *output = PyObject_IsTrue(const_cast<PyObject*>(object));
    return !PyErr_Occurred();
}

bool medPython_convert(int value, PyObject** output)
{
    *output = PyLong_FromLong(static_cast<long>(value));
    return *output;
}

bool medPython_convert(const PyObject* object, long* output)
{
    *output = PyLong_AsLong(const_cast<PyObject*>(object));
    return !PyErr_Occurred();
}

bool medPython_convert(double value, PyObject** output)
{
    *output = PyFloat_FromDouble(value);
    return *output;
}

bool medPython_convert(const PyObject* object, double* output)
{
    *output = PyFloat_AsDouble(const_cast<PyObject*>(object));
    return !PyErr_Occurred();
}
