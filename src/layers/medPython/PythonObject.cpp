#include "PythonObject.h"

#include "Exception.h"
#include "swig_runtime.h"
#include "PythonType.h"

namespace medPython
{

class PythonObject
{
public:
    PyObject* object;
};

PythonObject PythonObject::borrowed(PyObject* object)
{
    PythonObject borrowedObject;
    borrowedObject.internalBorrow(object);
    return borrowedObject;
}

PythonObject::PythonObject(PyObject* object) :
    d(new PythonObject)
{
    setObject(object);
}

PythonObject::PythonObject(QString string) :
    d(new PythonObject)
{
    setObject(PyUnicode_FromString(TO_CSTRING(string)));
    CHECK_PYTHON_ERROR();
}

PythonObject::PythonObject(const PythonObject& other) :
    d(new PythonObject)
{
    internalBorrow(other.d->object);
}

PythonObject& PythonObject::operator=(const PythonObject& other)
{
    borrow(other.d->object);
    return *this;
}

PythonObject::~PythonObject()
{
    clear();
    delete d;
}

void PythonObject::clear()
{
    Py_DECREF(d->object);
    setObject(nullptr);
}

PythonObject::operator bool() const
{
    return (d->object != Py_None);
}

bool PythonObject::operator!() const
{
    return (d->object == Py_None);
}

void PythonObject::borrow(PyObject* object)
{
    internalBorrow(object);
}

PyObject* PythonObject::data()
{
    return d->object;
}

const PyObject*PythonObject::data() const
{
    return d->object;
}

PyObject* PythonObject::takeReference()
{
    PyObject* object = d->object;
    setObject(nullptr);
    return object;
}

PyObject* PythonObject::newReference()
{
    Py_INCREF(d->object);
    return d->object;
}

PythonType PythonObject::getType()
{
    PythonType type = PyObject_Type(data());
    CHECK_PYTHON_ERROR();
    return type;
}

template<class TYPE>
TYPE PythonObject::cast()
{
    return internalCast<TYPE>(false);
}

PythonObject PythonObject::getAttribute(QString name)
{
    PythonObject value = PyObject_GetAttrString(data(), TO_CSTRING(name));
    CHECK_PYTHON_ERROR();
    return value;
}

void PythonObject::setAttribute(QString name, PythonObject value)
{
    PyObject_SetAttrString(data(), TO_CSTRING(name), value);
    CHECK_PYTHON_ERROR();
}

PythonObject PythonObject::callMethod(QString name, PythonObject args, PythonObject kwargs)
{
    PythonObject method = getAttribute(name);
    return method.call(args, kwargs);
}

PythonObject PythonObject::call(PythonObject args, PythonObject kwargs)
{
    PythonObject result;

    if (args)
    {
        if (kwargs)
        {
            result = PyObject_Call(data(), args.data(), kwargs.data());
        }
        else
        {
            result = PyObject_CallObject(data(), args.data());
        }
    }
    else
    {
        if (kwargs)
        {
            PythonObject emptyTuple = PyTuple_New(0);
            result = PyObject_Call(data(), emptyTuple.data(), kwargs.data());
        }
        else
        {
            result = PyObject_CallNoArgs(data());
        }
    }

    CHECK_PYTHON_ERROR();
    return result;
}

QString PythonObject::toString()
{
    PythonObject stringAsPyBytes;
    QString result;

    if (PyBytes_Check(data()))
    {
        stringAsPyBytes = this->newReference();
    }
    else if (PyUnicode_Check(data()))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(data(), "UTF-8", "strict");
        CHECK_PYTHON_ERROR();
    }

    if (stringAsPyBytes)
    {
        result = strdup(PyBytes_AS_STRING(stringAsPyBytes.data()));
    }
    else
    {
        // object is not a string so apply str() and retry
        result = PythonObject(PyObject_Str(data())).toString();
    }
}

void PythonObject::setObject(PyObject* object)
{
    if (object)
    {
        d->object = object;
    }
    else
    {
        d->object = Py_None;
        Py_INCREF(d->object);
    }
}

void PythonObject::internalBorrow(PyObject* object)
{
    Py_XINCREF(object);
    Py_DECREF(d->object);
    setObject(object);
}

template<class TYPE>
medPython::PythonObject::TYPE* PythonObject::internalCast(bool disown)
{
    QString castTypeName = TYPE::staticMetaObject.className();

    swig_type_info* swigType = SWIG_TypeQuery(TO_CSTRING(castTypeName));

    if (!swigType)
    {
        THROW_PYTHON_RELATED_EXCEPTION("No wrapping found for " + castTypeName);
    }

    TYPE* castedInstance;
    int conversionResult = SWIG_Python_ConvertPtr(d->object, &castedInstance, swigType, disown ? SWIG_POINTER_DISOWN : 0);

    if (SWIG_IsOK(conversionResult))
    {
        return castedInstance;
    }
    else
    {
        THROW_PYTHON_RELATED_EXCEPTION("Cannot cast object of type " + Py_TYPE(d->object)->tp_name + " to " + castTypeName);
    }
}

}
