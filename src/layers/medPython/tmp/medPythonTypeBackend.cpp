#include "medPythonTypeBackend.h"

namespace med
{

PythonTypeBackend::PythonTypeBackend(PyObject* object) :
    object(object)
{
}

void PythonTypeBackend::setWrongTypeError(QString typeName)
{
    QString message = "Object of type " + Py_TYPE(object)->tp_name + " is not a " + typeName;
    PyErr_SetString(PyExc_ValueError, qUtf8Printable(message));
}

}
