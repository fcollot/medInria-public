#pragma once

#include "medTypeCheckedPythonObject.h"

namespace med
{

int isPythonListObject(PyObject* object);

class MEDPYTHON_EXPORT PythonList : public TypeCheckedPythonObject<&isPythonListObject>
{
public:
    static PythonList create(QList<PythonObject> items = {});
    static PythonList create(QList<QString> items);

    PythonList(PyObject* listObject = nullptr);
    PythonList(const PythonObject& other);

    bool isEmpty();
    int getSize();

    PythonObject getItem(int i);

    void append(PythonObject item);
    void append(QString item);

    void prepend(PythonObject item);
    void prepend(QString item);

    void remove(PythonObject item);
    void remove(QString item);
};

}
