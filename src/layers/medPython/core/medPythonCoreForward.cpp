/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonCoreForward.h"

#include "medPythonCore.h"

namespace med::python
{

void incref(PyObject* object)
{
    Py_XINCREF(object);
}

void decref(PyObject* object)
{
    Py_CLEAR(object);
}

PyObject* none()
{
    Py_RETURN_NONE;
}

} // namespace med::python
