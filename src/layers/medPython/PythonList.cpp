#include "PythonList.h"

#include <QList>

#include "ExceptionManager.h"

namespace medPython
{

PythonList::PythonList(PyObject* listObject) :
    TypeCheckedPythonObject(listObject)
{
}

PythonList::PythonList(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

PythonList::PythonList(QList<PythonObject> items) :
    TypeCheckedPythonObject(PyList_New(items.length()))
{
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(data(), i, items[i].newReference());
    }
}

PythonList::PythonList(QList<QString> items) :
    TypeCheckedPythonObject(PyList_New(items.length()))
{
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(data(), i, PythonObject(items[i]).newReference());
    }
}

bool PythonList::isEmpty()
{
    return getSize() > 0;
}

int PythonList::getSize()
{
    int size = PyList_Size(data());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonList::getItem(int i)
{
    PythonObject item = PythonObject::borrowed(PyList_GetItem(data(), i));
    MEDPYTHON_CHECK_ERROR();
    return item;
}

void PythonList::append(PythonObject item)
{
    PyList_Append(data(), item.newReference());
    MEDPYTHON_CHECK_ERROR();
}

void PythonList::append(QString item)
{
    append(PythonObject(item));
}

}
