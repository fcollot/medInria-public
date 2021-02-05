#include "medPythonSequenceBackend.h"

namespace med
{

PythonSequenceBackend::PythonSequenceBackend(PyObject* object) :
    PythonTypeBackend(object)
{
    if (!PySequence_Check(object))
    {
        QString message = "Object of type " + Py_TYPE(object)->tp_name + " is not a sequence";
        PyErr_SetString(PyExc_ValueError, qUtf8Printable(message));
    }
}

PyObject* PythonSequenceBackend::toList()
{
    return PySequence_List(sequence);
}

PyObject* PythonSequenceBackend::toTuple()
{
    return PySequence_Tuple(sequence);
}

PyObject* PythonSequenceBackend::getItem(size_t i)
{
    return PySequence_GetItem(sequence, i);
}

PythonSequenceReference PythonSequenceBackend::getSequenceReference(size_t i)
{
    return PythonSequenceReference(*this, i); // need copy constructor
}

}
