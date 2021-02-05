#include "medPythonSequenceBackend.h"

namespace med
{

PythonSequenceBackend::PythonSequenceBackend(PyObject* object) :
    sequence(object)
{
    if (!PySequence_Check(object))
    {
        QString message = "Object of type " + Py_TYPE(object)->tp_name + " is not a sequence";
        PyErr_SetString(PyExc_ValueError, qUtf8Printable(message));
    }
}

PythonObject PythonSequenceBackend::getItem(size_t i)
{
    PythonObject item = PythonObject::take(PySequence_GetItem(sequence, i));
    MEDPYTHON_CHECK_ERROR();
    return item;
}

void PythonSequenceBackend::insertItem(size_t i, PythonObject& value)
{

}

}
