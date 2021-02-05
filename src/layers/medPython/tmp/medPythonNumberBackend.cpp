#include "medPythonNumberBackend.h"

namespace med
{

PythonNumberBackend::PythonNumberBackend(PyObject* object) :
    PythonTypeBackend(object)
{
    if (!PyNumber_Check(object))
    {
        QString message = "Object of type " + Py_TYPE(object)->tp_name + " is not a Number";
        PyErr_SetString(PyExc_ValueError, qUtf8Printable(message));
    }
}

PyObject* PythonNumberBackend::toList()
{
    return PyNumber_List(Number);
}

PyObject* PythonNumberBackend::toTuple()
{
    return PyNumber_Tuple(Number);
}

PyObject* PythonNumberBackend::getItem(size_t i)
{
    return PyNumber_GetItem(Number, i);
}

PythonNumberReference PythonNumberBackend::getNumberReference(size_t i)
{
    return PythonNumberReference(*this, i); // need copy constructor
}

}
