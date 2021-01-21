#pragma once

#include "TypeCheckedPythonObject.h"

namespace medPython
{

int isTupleObject(PyObject* object)
{
    return PyTuple_Check(object);
}

class MEDPYTHON_EXPORT PythonTuple : public TypeCheckedPythonObject<&isTupleObject>
{
public:
    PythonTuple(PyObject* tupleObject = nullptr);
    PythonTuple(const PythonObject& other);
    PythonTuple(QList<PythonObject> items);

    int getSize();

    PythonObject getItem(int i);
    void setItem(int i, PythonObject item);
};

}
