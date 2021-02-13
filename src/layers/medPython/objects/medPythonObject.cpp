#include "medPythonObject.h"

#include "medPythonCore.h"

namespace med
{
namespace python
{

struct ObjectPrivate
{
    PyObject* reference;
};

Object::Object(PyObject* reference) :
    d(new ObjectPrivate)
{
    internalSetReference(reference);
}

Object::Object(const Object& other) :
    d(new ObjectPrivate)
{
    internalSetReference(other.newReference());
}

Object::Object(const AbstractObject& other) :
    d(new ObjectPrivate)
{
    internalSetReference(other.newReference());
}

Object::Object(int value) :
    d(new ObjectPrivate)
{
    PyObject* reference;

    if (!convertToPython(value, &reference))
    {
        propagateCurrentError();
    }

    internalSetReference(reference);
}

Object::Object(double value) :
    d(new ObjectPrivate)
{
    PyObject* reference;

    if (!convertToPython(value, &reference))
    {
        propagateCurrentError();
    }

    internalSetReference(reference);
}

Object::Object(QString value) :
    d(new ObjectPrivate)
{
    PyObject* reference;

    if (!convertToPython(value, &reference))
    {
        propagateCurrentError();
    }

    internalSetReference(reference);
}

Object::~Object()
{
    if (Py_IsInitialized())
    {
        Py_CLEAR(d->reference);
    }

    delete d;
}

PyObject* Object::getReference() const
{
    return d->reference;
}

void Object::setReference(PyObject* reference)
{
    PyObject* oldReference = d->reference;
    internalSetReference(reference);
    Py_CLEAR(oldReference);
}

void Object::internalSetReference(PyObject* reference)
{
    if (reference)
    {
        d->reference = reference;
    }
    else
    {
        d->reference = Py_None;
        Py_INCREF(d->reference);
    }
}

} // namespace python
} // namespace med
