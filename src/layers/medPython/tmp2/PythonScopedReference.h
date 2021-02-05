#pragma once

#include "medPythonCore.h"
#include "medPythonExport.h"

namespace med
{

class PythonScopedReference
{
public:
    PythonScopedReference(PyObject* reference, bool isNew = true);
    ~PythonScopedReference();

private:
    PyObject* reference;
};

}
