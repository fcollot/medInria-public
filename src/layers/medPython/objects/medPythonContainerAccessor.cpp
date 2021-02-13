#include "medPythonContainerAccessor.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
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
    coreFunctionNoReturn<&PyObject_SetItem, PyObject*, PyObject*>(*d->container, *d->key, reference);
    Py_CLEAR(reference);
}

} // namespace python
} // namespace med
