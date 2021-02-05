#pragma once

#include "medPythonContainerBackend.h"

namespace med
{

class MEDPYTHON_EXPORT PythonSequenceBackend : public PythonContainerBackend
{
public:
    PythonSequenceBackend(PyObject* object);

    PythonObject getItem(PythonObject& container, PythonObject& key);

    PythonContainerReference getModifiableReference(PythonObject& container, PythonObject& key);
};

}
