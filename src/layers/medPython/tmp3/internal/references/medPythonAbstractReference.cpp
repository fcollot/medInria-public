#include "medPythonAbstractReference.h"

namespace med
{
namespace python
{
namespace internal
{

PyObject* AbstractReference::newReference() const
{
    PyObject* object = get();
    Py_XINCREF(object);
    return object;
}

bool AbstractReference::isNull()
{
    return (get() == nullptr);
}

}
}
}
