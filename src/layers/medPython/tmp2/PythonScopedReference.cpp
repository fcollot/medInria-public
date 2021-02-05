#include "PythonScopedReference.h"

namespace med
{

PythonScopedReference::PythonScopedReference(PyObject* reference, bool isNew) :
    reference(reference)
{
    if (!isNew)
    {
        Py_INCREF(reference);
    }
}

PythonScopedReference::~PythonScopedReference()
{
    Py_DECREF(reference);
}

}
