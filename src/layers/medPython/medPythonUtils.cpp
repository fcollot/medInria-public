#include "medPythonUtils.h"

#include "medPythonCore.h"

namespace med
{
namespace python
{
namespace utils
{

void incref(PyObject* object)
{
    Py_INCREF(object);
}

void decref(PyObject* object)
{
    Py_CLEAR(object);
}

PyObject* createList(ssize_t length)
{
    return PyList_New(length);
}

PyObject* createDict()
{
    return PyDict_New();
}

PyObject* getItem(PyObject* sequence, ssize_t index)
{
    return PySequence_GetItem(sequence, index);
}

void setItem(PyObject* sequence, ssize_t index, PyObject* value)
{
    PySequence_SetItem(sequence, index, value);
}

PyObject* getItem(PyObject* mapping, PyObject* key)
{
    return PyObject_GetItem(mapping, key);
}

void setItem(PyObject* mapping, PyObject* key, PyObject* value)
{
    PyObject_SetItem(mapping, key, value);
}

ssize_t length(PyObject* container)
{
    return PyObject_Length(container);
}

PyObject* keys(PyObject* mapping)
{
    return PyMapping_Keys(mapping);
}

PyObject* values(PyObject* mapping)
{
    return PyMapping_Values(mapping);
}

PyObject* call(PyObject* callable, PyObject* args, PyObject* kwargs)
{
    ssize_t numArgs = args ? length(args) : 0;

    if (PyErr_Occurred())
    {
        return nullptr;
    }

    ssize_t numKwargs = kwargs ? length(kwargs) : 0;

    if (PyErr_Occurred())
    {
        return nullptr;
    }

    if (numKwargs > 0)
    {
        return PyObject_Call(callable, args, kwargs);
    }
    else
    {
        if (numArgs > 0)
        {
            return PyObject_CallObject(callable, args);
        }
        else
        {
            return PyObject_CallNoArgs(callable);
        }
    }
}

PyObject* callMethod(PyObject* object, QString method, PyObject* args, PyObject* kwargs)
{
    PyObject* methodObject = PyObject_GetAttrString(object, qUtf8Printable(method));

    if (methodObject)
    {
        return call(methodObject, args, kwargs);
    }
    else
    {
        return nullptr;
    }
}

QString toString(PyObject* object)
{
    PyObject* stringAsPyBytes = nullptr;
    QString result;

    if (PyBytes_Check(object))
    {
        stringAsPyBytes = object;
        incref(object);
    }
    else if (PyUnicode_Check(object))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(object, "UTF-8", "strict");
    }

    if (stringAsPyBytes)
    {
        result = strdup(PyBytes_AS_STRING(stringAsPyBytes));
        decref(stringAsPyBytes);
    }
    else
    {
        // The object is not a string, so apply str() and retry
        PyObject* stringObject = PyObject_Str(object);

        if (stringObject)
        {
            result = toString(stringObject);
            decref(stringObject);
        }
    }

    return result;
}

PyObject* fromString(QString string)
{
    return PyUnicode_FromString(qUtf8Printable(string));
}

}
}
}
