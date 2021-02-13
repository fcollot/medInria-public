/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonContainerAccessor.h"

#include "medPythonCoreWrappers.h"

namespace med::python
{

struct ContainerAccessorPrivate
{
    Object container;
    Object key;
    Object value;
};

ContainerAccessor::ContainerAccessor(const AbstractObject& container, PyObject* key) :
    d(new ContainerAccessorPrivate)
{
    d->container = container;
    d->key = key;
}

PyObject* ContainerAccessor::getReference() const
{
    d->value = coreFunction<PyObject*, &PyObject_GetItem, PyObject*, PyObject*>(*d->container, *d->key);
    return *d->value;
}

void ContainerAccessor::setReference(PyObject* reference)
{
    d->value = reference;
    coreFunctionNoReturn<&PyObject_SetItem, PyObject*, PyObject*>(*d->container, *d->key, *d->value);
}

} // namespace med::python
