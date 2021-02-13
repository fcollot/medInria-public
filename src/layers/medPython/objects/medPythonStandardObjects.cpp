/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonStandardObjects.h"

#include "medPythonCoreWrappers.h"

namespace med::python
{

PyObject* none()
{
    Py_RETURN_NONE;
}

Object list()
{
    return coreFunction<PyObject*, &PyList_New, ssize_t>(0);
}

Object list(const Object& object)
{
    return coreFunction<PyObject*, &PySequence_List, PyObject*>(*object);
}

Object tuple()
{
    return coreFunction<PyObject*, &PyTuple_New, ssize_t>(0);
}

Object tuple(const Object& object)
{
    return coreFunction<PyObject*, &PySequence_Tuple, PyObject*>(*object);
}

Object dict()
{
    return coreFunction<PyObject*, &PyDict_New>();
}

} // namespace med::python
