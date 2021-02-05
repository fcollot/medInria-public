#pragma once

#include "medTypeCheckedPythonObject.h"

namespace med
{

int isDictObject(PyObject* object);

class MEDPYTHON_EXPORT PythonDict : public TypeCheckedPythonObject<&isDictObject>
{
public:
    static PythonDict create(QHash<PythonObject, PythonObject> items);
    static PythonDict create(QHash<QString, PythonObject> items = {});

    PythonDict(PyObject* DictObject = nullptr);
    PythonDict(const PythonObject& other);

    int getSize();

    PythonObject getItem(PythonObject key);
    PythonObject getItem(QString key);
    void setItem(PythonObject key, PythonObject value);
    void setItem(QString key, PythonObject value);
};

}
