#include "medPythonListBackend.h"

namespace med
{

PythonListBackend::PythonListBackend(PyObject* object) :
    list(object)
{
    if (!PyList_Check(object))
    {
        QString message = "Object of type " + Py_TYPE(object)->tp_name + " is not a list";
        PyErr_SetString(PyExc_ValueError, qUtf8Printable(message));
    }
}

void PythonListBackend::insertItem(size_t i, PythonObject& value)
{

}

}
