#pragma once

#include "medPythonTypeBackend.h"

namespace med
{

class MEDPYTHON_EXPORT PythonNumberBackend : public PythonTypeBackend
{
public:
    PythonNumberBackend(PyObject* object);

    PyObject
};

}
