#include "medPythonList.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{

ssize_t Container::length() const
{
    return coreFunction<ssize_t, &PyObject_Length, PyObject*>(getNativeReference());
}

bool Container::contains(const Reference& object) const
{
    if (PyMapping_Check(**this))
    {
        return coreFunction<int, &PyMapping_HasKey, PyObject*, PyObject*>(getNativeReference(), object.getNativeReference());
    }
    else
    {
        return coreFunction<int, &PySequence_Contains, PyObject*, PyObject*>(getNativeReference(), getNativeReference());
    }
}

} // namespace python
} // namespace med
