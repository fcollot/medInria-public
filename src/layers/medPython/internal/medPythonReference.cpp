#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

Reference Reference::borrowed(PyObject* object)
{
    Py_INCREF(object);
    return object;
}

Reference::Reference(PyObject* nativeReference) :
    nativeReference(nativeReference)
{
}

Reference::~Reference()
{
    Py_XDECREF(nativeReference);
}

Reference& Reference::operator=(const AbstractReference& other)
{
    return this->operator=(other.newReference());
}

Reference& Reference::operator=(PyObject* object)
{
    Py_XDECREF(nativeReference);
    nativeReference = object;
    return *this;
}

PyObject* Reference::get() const
{
    return nativeReference;
}

void Reference::borrow(PyObject* object)
{
    Py_INCREF(object);
    this->operator=(object);
}

}
}
}
