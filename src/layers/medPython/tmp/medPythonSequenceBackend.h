#pragma once

#include "medPythonTypeBackend.h"

namespace med
{

class MEDPYTHON_EXPORT PythonSequenceBackend : public PythonTypeBackend
{
public:
    PythonSequenceBackend(PyObject* object);

    PyObject* toList();

    PyObject* toTuple();

    PyObject* getItem(size_t i);

    PythonSequenceReference getSequenceReference(size_t i);

    void insertItem(size_t i, PythonObject& value);

    void deleteItem(size_t i);

    size_t countItem(PythonObject& object);

    void extend(PythonObject& otherSequence);

    size_t indexOfFirstOccurence(PythonObject& object);
};

}
