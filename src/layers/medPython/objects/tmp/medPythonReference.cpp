#include "medPythonReference.h"

#include "medPythonCore.h"

namespace med
{
namespace python
{

struct ReferencePrivate
{
    PyObject* nativeReference;
};

Reference::Reference(PyObject* nativeReference) :
    d(new ReferencePrivate)
{
    d->nativeReference = nativeReference;
}

Reference::~Reference()
{
    Py_CLEAR(d->nativeReference);
    delete d;
}

PyObject* Reference::get() const
{
    return d->nativeReference;
}

void Reference::set(PyObject* nativeReference)
{

}

} // namespace python
} // namespace med
