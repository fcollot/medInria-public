#include "medPythonAbstractObject.h"

#include "medPythonCoreWrappers.h"
#include "medPythonExceptions.h"
#include "medPythonObject.h"

namespace med
{
namespace python
{

AbstractObject& AbstractObject::operator=(const AbstractObject& other)
{
    setReference(other.newReference());
    return *this;
}

AbstractObject& AbstractObject::operator=(PyObject* reference)
{
    setReference(reference);
    return *this;
}

PyObject* AbstractObject::operator*() const
{
    return getReference();
}

PyObject* AbstractObject::newReference() const
{
    PyObject* reference = getReference();
    Py_INCREF(reference);
    return reference;
}

void AbstractObject::borrow(const PyObject* reference)
{
    Py_INCREF(reference);
    setReference(const_cast<PyObject*>(reference));
}

Object AbstractObject::type() const
{
    return coreFunction<PyObject*, &PyObject_Type, PyObject*>(**this);
}

QString AbstractObject::typeName() const
{
    return Py_TYPE(**this)->tp_name;
}

bool AbstractObject::hasAttribute(QString name) const
{
    return coreFunction<int, &PyObject_HasAttrString, PyObject*, const char*>(**this, qUtf8Printable(name));
}

Object AbstractObject::getAttribute(QString name) const
{
    return coreFunction<PyObject*, &PyObject_GetAttrString, PyObject*, const char*>(**this, qUtf8Printable(name));
}

void AbstractObject::setAttribute(QString name, AbstractObject& value)
{
    coreFunctionNoReturn<&PyObject_SetAttrString, PyObject*, const char*, PyObject*>(**this, qUtf8Printable(name), *value);
}

QList<QString> AbstractObject::dir() const
{
    Object dir = coreFunction<PyObject*, &PyObject_Dir, PyObject*>(**this);
    return dir.convert<QList<QString> >();
}

AbstractObject::operator bool() const
{
    return coreFunction<bool, &PyObject_IsTrue, PyObject*>(**this);
}

bool AbstractObject::operator!() const
{
    return coreFunction<int, &PyObject_Not, PyObject*>(**this);
}

bool AbstractObject::operator==(const AbstractObject& other) const
{
    return coreFunction<bool, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_EQ);
}

bool AbstractObject::operator!=(const AbstractObject& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_NE);
}

bool AbstractObject::operator<(const AbstractObject& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LT);
}

bool AbstractObject::operator>(const AbstractObject& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GT);
}

bool AbstractObject::operator<=(const AbstractObject& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LE);
}

bool AbstractObject::operator>=(const AbstractObject& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GE);
}

Object AbstractObject::operator+(const AbstractObject& other) const
{
    return coreFunction<PyObject*, &PyNumber_Add, PyObject*, PyObject*>(**this, *other);
}

Object AbstractObject::operator-(const AbstractObject& other) const
{
    return coreFunction<PyObject*, &PyNumber_Subtract, PyObject*, PyObject*>(**this, *other);
}

Object AbstractObject::operator*(const AbstractObject& other) const
{
    return coreFunction<PyObject*, &PyNumber_Multiply, PyObject*, PyObject*>(**this, *other);
}

Object AbstractObject::operator/(const AbstractObject& other) const
{
    return coreFunction<PyObject*, &PyNumber_TrueDivide, PyObject*, PyObject*>(**this, *other);
}

AbstractObject& AbstractObject::operator+=(const AbstractObject& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceAdd, PyObject*, PyObject*>(**this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator-=(const AbstractObject& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceSubtract, PyObject*, PyObject*>(**this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator*=(const AbstractObject& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceMultiply, PyObject*, PyObject*>(**this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator/=(const AbstractObject& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceTrueDivide, PyObject*, PyObject*>(**this, *other));
    return *this;
}

ssize_t AbstractObject::length() const
{
    return coreFunction<ssize_t, &PyObject_Length, PyObject*>(**this);
}

bool AbstractObject::contains(const AbstractObject& object) const
{
    if (PyMapping_Check(**this))
    {
        return coreFunction<int, &PyMapping_HasKey, PyObject*, PyObject*>(**this, *object);
    }
    else if (PySequence_Check(**this))
    {
        return coreFunction<int, &PySequence_Contains, PyObject*, PyObject*>(**this, *object);
    }
    else
    {
        throw TypeError(QString("%1 does not support %2").arg((typeName())).arg(__func__));
    }
}

void AbstractObject::append(const AbstractObject& item)
{
    if (PyList_Check(**this))
    {
        coreFunctionNoReturn<&PyList_Append, PyObject*, PyObject*>(**this, *item);
    }
    else
    {
        throw TypeError(QString("%1 does not support %2").arg((typeName())).arg(__func__));
    }
}

Object AbstractObject::keys() const
{
    return coreFunction<PyObject*, &PyMapping_Keys, PyObject*>(**this);
}

Object AbstractObject::values() const
{
    return coreFunction<PyObject*, &PyMapping_Values, PyObject*>(**this);
}

void AbstractObject::update(const AbstractObject& other)
{
    if (PyDict_Check(**this))
    {
        coreFunctionNoReturn<&PyDict_Merge, PyObject*, PyObject*, const int&>(**this, *other, 1);
    }
    else
    {
        throw TypeError(QString("%1 does not support %2").arg((typeName())).arg(__func__));
    }
}

} // namespace python
} // namespace med
