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

AbstractReference::operator bool()
{
    return (get() == nullptr);
}

}
}
}
