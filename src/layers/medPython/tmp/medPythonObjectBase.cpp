#include "medPythonObjectBase.h"

#include "medPythonExceptionManager.h"

namespace med
{

class PythonObjectBasePrivate
{
public:
    PyObject* reference;
};

PythonObjectBase::PythonObjectBase(const PythonObjectBase& other) :
    d(new PythonObjectBasePrivate)
{
    internalBorrow(other.d->reference);
}

PythonObjectBase& PythonObjectBase::operator=(const PythonObjectBase& other)
{
    internalBorrow(other.d->reference);
    return *this;
}

PyObject* PythonObjectBase::getReference()
{
    return d->reference;
}

PyObject* PythonObjectBase::takeReference()
{
    PyObject* object = d->reference;
    d->reference = Py_None;
    Py_INCREF(d->reference);
    return object;
}

PyObject* PythonObjectBase::newReference()
{
    Py_INCREF(d->reference);
    return d->reference;
}

PythonObjectBase PythonObjectBase::type()
{
    PyObject* result = Py_TYPE(d->reference);
    Py_INCREF(result);
    return result;
}

QString PythonObjectBase::typeName()
{
    return Py_TYPE(d->reference)->tp_name;
}

bool PythonObjectBase::hasAttribute(QString name)
{
    return PyObject_HasAttrString(d->reference, qUtf8Printable(name));
}

PythonObjectBase PythonObjectBase::getAttribute(QString name)
{
    PyObject* result = PyObject_GetAttrString(d->reference, qUtf8Printable(name));
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

void PythonObjectBase::setAttribute(QString name, PythonObjectBase value)
{
    PyObject_SetAttrString(d->reference, qUtf8Printable(name), value.d->reference);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
}

PythonObjectBase::PythonObjectBase(PyObject* object) :
    d(new PythonObjectBasePrivate)
{
    d->reference = object;
}

PythonObjectBase::~PythonObjectBase()
{
    Py_DECREF(d->reference);
    delete d;
}

void PythonObjectBase::setReference(PyObject* object)
{
    d->reference = object;
}

void PythonObjectBase::replaceReference(PyObject* object)
{
    Py_DECREF(d->reference);
    d->reference = object;
}

void PythonObjectBase::internalBorrow(PyObject* object)
{
    Py_XINCREF(object);
    Py_DECREF(d->reference);
    d->reference = object;
}

}
