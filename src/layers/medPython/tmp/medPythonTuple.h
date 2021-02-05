#pragma once

#include "medTypeCheckedPythonObject.h"

namespace med
{

int isTupleObject(PyObject* object);

class MEDPYTHON_EXPORT PythonTuple : public TypeCheckedPythonObject<&isTupleObject>
{
public:
    static PythonTuple create(QList<PythonObject> items = {});

    PythonTuple(PyObject* tupleObject = nullptr);
    PythonTuple(const PythonObject& other);

    int getSize();

    PythonObject getItem(int i);
    void setItem(int i, PythonObject item);
};

}
