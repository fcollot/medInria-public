#pragma once

#include "TypeCheckedPythonObject.h"

namespace medPython
{

int isListObject(PyObject* object)
{
    return PyList_Check(object);
}

class MEDPYTHON_EXPORT PythonList : public TypeCheckedPythonObject<&isListObject>
{
public:
    PythonList(PyObject* listObject = nullptr);
    PythonList(const PythonObject& other);
    PythonList(QList<PythonObject> items);
    PythonList(QList<QString> items);

    bool isEmpty();
    int getSize();

    PythonObject getItem(int i);
    void append(PythonObject item);
    void append(QString item);
};

}
