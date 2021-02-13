#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

// These functions replicate the equivalent Python functions to create built-in
// objects (for numeric and string objects, use the corresponding Object
// constructors directly)

#include <QHash>

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med::python
{

MEDPYTHON_EXPORT PyObject* none();

MEDPYTHON_EXPORT Object list();

MEDPYTHON_EXPORT Object list(const Object& object);

template <class TYPE>
Object list(QList<TYPE> qList)
{
    return Object::create(qList);
}

MEDPYTHON_EXPORT Object tuple();

MEDPYTHON_EXPORT Object tuple(const Object& object);

template <class TYPE>
Object tuple(QList<TYPE> qList)
{
    return tuple(Object::create(qList));
}

MEDPYTHON_EXPORT Object dict();

template <class KEY_TYPE, class VALUE_TYPE>
Object dict(QHash<KEY_TYPE, VALUE_TYPE> qHash)
{
    return Object::create<QHash<KEY_TYPE, VALUE_TYPE> >(qHash);
}

} // namespace med::python
