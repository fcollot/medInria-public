#include "medPythonObject.h"

#include "medPythonContainerConversions.h"
#include "medPythonCore.h"
#include "medPythonCoreWrappers.h"
#include "medPythonPrimitiveConversions.h"

namespace med
{
namespace python
{

class ObjectPrivate
{
public:
    PyObject* reference;
};

Object Object::borrowed(const PyObject* nativeObject)
{
    Py_INCREF(nativeObject);
    return const_cast<PyObject*>(nativeObject);
}

Object::Object(double value) :
    d(new ObjectPrivate)
{
    long intValue = static_cast<long>(value);

    if (intValue == value)
    {
        d->reference = Convert<long>::toPython(intValue);
    }
    else
    {
        d->reference = Convert<double>::toPython(value);
    }
}

Object::Object(QString value) :
    d(new ObjectPrivate)
{
    d->reference = Convert<QString>::toPython(value);
}

Object::Object(const Object& other) :
    d(new ObjectPrivate)
{
    d->reference = other.newReference();
}

Object& Object::operator=(const Object& other)
{
    setReference(other.newReference());
    return *this;
}

Object::Object(PyObject* nativeObject) :
    d(new ObjectPrivate)
{
    if (nativeObject)
    {
        d->reference = nativeObject;
    }
    else
    {
        d->reference = Py_None;
        Py_INCREF(d->reference);
    }
}

Object& Object::operator=(PyObject* nativeObject)
{
    setReference(nativeObject);
    return *this;
}

Object::~Object()
{
    Py_CLEAR(d->reference);
    delete d;

    if (dynamic_cast<FunctionCall<Object>* >(this))
    {
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
    }
}

PyObject* Object::operator*() const
{
    return getReference();
}

void Object::borrow(const PyObject* nativeObject)
{
    Py_INCREF(nativeObject);
    setReference(const_cast<PyObject*>(nativeObject));
}

PyObject* Object::newReference() const
{
    PyObject* reference = getReference();
    Py_INCREF(reference);
    return reference;
}

Object Object::type() const
{
    return coreFunction<PyObject*, &PyObject_Type, PyObject*>(getReference());
}

QString Object::typeName() const
{
    return Py_TYPE(**this)->tp_name;
}

bool Object::hasAttribute(QString name) const
{
    return coreFunction<int, &PyObject_HasAttrString, PyObject*, const char*>(**this, qUtf8Printable(name));
}

Object Object::getAttribute(QString name) const
{
    return coreFunction<PyObject*, &PyObject_GetAttrString, PyObject*, const char*>(**this, qUtf8Printable(name));
}

void Object::setAttribute(QString name, Object& value)
{
    coreFunctionNoReturn<&PyObject_SetAttrString, PyObject*, const char*, PyObject*>(**this, qUtf8Printable(name), *value);
}

QList<QString> Object::dir() const
{
    Object dir = coreFunction<PyObject*, &PyObject_Dir, PyObject*>(**this);
    return Convert<QList<QString> >::fromPython(*dir);
}

Object::operator bool() const
{
    return coreFunction<bool, &PyObject_IsTrue, PyObject*>(**this);
}

bool Object::operator!() const
{
    return coreFunction<int, &PyObject_Not, PyObject*>(**this);
}

bool Object::operator==(Object& other) const
{
    return coreFunction<bool, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_EQ);
}

bool Object::operator!=(Object& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_NE);
}

bool Object::operator<(Object& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LT);
}

bool Object::operator>(Object& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GT);
}

bool Object::operator<=(Object& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LE);
}

bool Object::operator>=(Object& other) const
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GE);
}

Object Object::operator+(Object& other) const
{
    return coreFunction<PyObject*, &PyNumber_Add, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator-(Object& other) const
{
    return coreFunction<PyObject*, &PyNumber_Subtract, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator*(Object& other) const
{
    return coreFunction<PyObject*, &PyNumber_Multiply, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator/(Object& other) const
{
    return coreFunction<PyObject*, &PyNumber_TrueDivide, PyObject*, PyObject*>(**this, *other);
}

Object& Object::operator+=(Object& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceAdd, PyObject*, PyObject*>(**this, *other));
    return *this;
}

Object& Object::operator-=(Object& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceSubtract, PyObject*, PyObject*>(**this, *other));
    return *this;
}

Object& Object::operator*=(Object& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceMultiply, PyObject*, PyObject*>(**this, *other));
    return *this;
}

Object& Object::operator/=(Object& other)
{
    setReference(coreFunction<PyObject*, &PyNumber_InPlaceTrueDivide, PyObject*, PyObject*>(**this, *other));
    return *this;
}

void Object::append(Object& item)
{
    coreFunctionNoReturn<&PyList_Append, PyObject*, PyObject*>(**this, *item);
}

ssize_t Object::length() const
{
    return coreFunction<ssize_t, &PyObject_Length, PyObject*>(**this);
}

bool Object::contains(Object& item) const
{
    if (PyMapping_Check(**this))
    {
        return coreFunction<int, &PyMapping_HasKey, PyObject*, PyObject*>(**this, *item);
    }
    else
    {
        return coreFunction<int, &PySequence_Contains, PyObject*, PyObject*>(**this, *item);
    }
}

Object Object::keys() const
{
    return coreFunction<PyObject*, &PyMapping_Keys, PyObject*>(**this);
}

Object Object::values() const
{
    return coreFunction<PyObject*, &PyMapping_Values, PyObject*>(**this);
}

void Object::update(Object& other)
{
    coreFunctionNoReturn<&PyDict_Merge, PyObject*, PyObject*, const int&>(**this, *other, 1);
}

SequenceAccessor<Object> Object::operator[](ssize_t index)
{
    return SequenceAccessor(*this, index);
}

PyObject* Object::getReference() const
{
    return const_cast<Object*>(this)->getReference();
}

PyObject* Object::getReference()
{
    return d->reference;
}

void Object::setReference(PyObject* reference)
{
    PyObject* oldReference = d->reference;
    d->reference = reference;
    Py_CLEAR(oldReference);
}

Object Object::createArgsList(QList<Object> args)
{
    return Convert<QList<Object> >::toPython(args);
}

}
}
