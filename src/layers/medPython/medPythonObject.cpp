#include "medPythonObject.h"

#include "medPythonPrimitiveConversions.h"
#include "medPythonGeneralFunctions.h"
#include "medPythonMappingItemReference.h"
#include "medPythonReference.h"
#include "medPythonSequenceItemReference.h"

namespace med
{
namespace python
{

//Object Object::take(PyObject* nativeObject)
//{
//    return nativeObject;
//}

//Object Object::borrow(PyObject* nativeObject)
//{
//    Py_XINCREF(nativeObject);
//    return nativeObject;
//}

Object::Object(PyObject* nativeObject) :
    reference(internal::Reference(nativeObject))
{
    if (!reference)
    {
        reference = internal::none();
    }
}

Object::Object(const Object& other) :
    reference(internal::Reference(other.reference))
{
}

Object& Object::operator=(const Object& other)
{
    reference = other.reference;
    return *this;
}

Object::~Object()
{
    // If this object is a function call reference, we must handle the case of an isolated function call (one whose
    // return value is not used, which means the evaluation of the call was not triggered).
    FunctionCallReference* functionCall = dynamic_cast<FunctionCallReference*>(reference.data());

    if (functionCall)
    {
        try
        {
            functionCall->ensureEvaluated();
        }
        catch ()
        {
            reference.reset();

            // Exceptions should never be thrown from destructors unless we are certain there is no stack-unwinding
            // process taking place for another exception (and this is the case for function call references)
            throw;
        }
    }
}

Object::Object(long value)
{
    reference = internal::Convert<long>::toPython(value);
}

Object::Object(double value)
{
    reference = internal::Convert<double>::toPython(value);
}

Object::Object(QString value)
{
    reference = internal::Convert<QString>::toPython(value);
}

PyObject* Object::operator*()
{
    return reference->get();
}

PyObject* Object::operator*() const
{
    return reference->get();
}

PyObject* Object::takeReference()
{
    PyObject* object = reference->get();
    reference = internal::none();
    return object;
}

PyObject* Object::newReference() const
{
    return reference->newReference();
}

Object Object::type()
{
    return internal::getType(reference);
}

QString Object::typeName()
{
    return internal::getTypeName(reference);
}

bool Object::hasAttribute(QString name)
{
    return internal::hasAttribute(reference, name);
}

Object Object::getAttribute(QString name)
{
    return internal::getAttribute(reference, name);
}

void Object::setAttribute(QString name, Object value)
{
    internal::setAttribute(reference, value.reference);
}

QStringList Object::dir()
{
    return internal::dir(reference);
}

Object::operator bool()
{
    return internal::isTrue(reference);
}

bool Object::operator!()
{
    return internal::isNot(reference);
}

bool Object::operator==(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_EQ);
}

bool Object::operator!=(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_NE);
}

bool Object::operator<(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LT);
}

bool Object::operator>(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GT);
}

bool Object::operator<=(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_LE);
}

bool Object::operator>=(Object& other)
{
    return coreFunction<int, &PyObject_RichCompareBool, PyObject*, PyObject*, int>(**this, *other, Py_GE);
}

Object Object::operator+(Object& other)
{
    return coreFunction<PyObject*, &PyNumber_Add, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator-(Object& other)
{
    return coreFunction<PyObject*, &PyNumber_Subtract, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator*(Object& other)
{
    return coreFunction<PyObject*, &PyNumber_Multiply, PyObject*, PyObject*>(**this, *other);
}

Object Object::operator/(Object& other)
{
    return coreFunction<PyObject*, &PyNumber_TrueDivide, PyObject*, PyObject*>(**this, *other);
}

Object& Object::operator+=(Object& other)
{
    reference = coreFunction<PyObject*, &PyNumber_InPlaceAdd, PyObject*, PyObject*>(**this, *other);
    return *this;
}

Object& Object::operator-=(Object& other)
{
    reference = coreFunction<PyObject*, &PyNumber_InPlaceSubtract, PyObject*, PyObject*>(**this, *other);
    return *this;
}

Object& Object::operator*=(Object& other)
{
    reference = coreFunction<PyObject*, &PyNumber_InPlaceMultiply, PyObject*, PyObject*>(**this, *other);
    return *this;
}

Object& Object::operator/=(Object& other)
{
    reference = coreFunction<PyObject*, &PyNumber_InPlaceTrueDivide, PyObject*, PyObject*>(**this, *other);
    return *this;
}

void Object::append(const Object& item)
{
    coreFunctionNoReturn<&PyList_Append, PyObject*, PyObject*>(**this, *item);
}

size_t Object::length() const
{
    return coreFunction<size_t, &PyObject_Length, PyObject*>(**this);
}

bool Object::contains(Object& item) const
{
    if (PyMapping_Check(*item))
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

Object Object::operator[](const Object& key)
{
    return new MappingItemReference(newReference(), key.newReference());
}

Object Object::operator[](size_t i)
{
    return new SequenceItemReference(newReference(), i);
}

Object::Object(AbstractReference* reference) :
    reference(reference)
{
}

}
}
