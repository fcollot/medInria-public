#pragma once

#include "TypeCheckedPythonObject.h"

namespace medPython
{

int isDictObject(PyObject* object)
{
    return PyDict_Check(object);
}

class MEDPYTHON_EXPORT PythonDict : public TypeCheckedPythonObject<&isDictObject>
{
public:
    PythonDict(PyObject* DictObject = nullptr);
    PythonDict(const PythonObject& other);
    PythonDict(QHash<PythonObject, PythonObject> items);
    PythonDict(QHash<QString, PythonObject> items);

    int getSize();

    PythonObject getItem(PythonObject key);
    PythonObject getItem(QString key);
    void setItem(PythonObject key, PythonObject value);
    void setItem(QString key, PythonObject value);
};

}
