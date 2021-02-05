#pragma once

#include "medPythonContainerBackend.h"

namespace med
{

class MEDPYTHON_EXPORT PythonListBackend : public PythonContainerBackend
{
public:
    PythonListBackend(PyObject* object);

    void insertItem(ssize_t i, PythonObject& value);
};

}
