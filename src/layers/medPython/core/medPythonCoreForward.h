#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

// This header forwards some elements of the CPython library to avoid having
// to link with it when including other medPython headers.

#include "medPythonExport.h"

#include <QString>

typedef struct _object PyObject;

namespace med::python
{

// Increases the reference count of a python object if it is non-null
MEDPYTHON_EXPORT void incref(PyObject* object);

// Decreases the reference count of a python object if it is non-null
MEDPYTHON_EXPORT void decref(PyObject* object);

} // namespace med::python
PyObject* createList(ssize_t length);

void setNewListItem(PyObject* list, ssize_t index, PyObject* value);

PyObject* getItem(PyObject* sequence, ssize_t index);
void setItem(PyObject* sequence, ssize_t index, PyObject* value);

PyObject* getItem(PyObject* mapping, PyObject* key);
void setItem(PyObject* mapping, PyObject* key, PyObject* value);

} // namespace core_forward
} // namespace python
} // namespace med
