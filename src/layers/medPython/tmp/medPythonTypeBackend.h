#pragma once

#include <QString>

#include "medPythonCore.h"
#include "medPythonExport.h"

namespace med
{

class MEDPYTHON_EXPORT PythonTypeBackend
{
protected:
    PyObject* object;

    PythonTypeBackend(PyObject* object);

    void setWrongTypeError(QString typeName);
};

}
