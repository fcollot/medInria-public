#include "PythonList.h"

#include <QList>

#include "Convert.h"
#include "Exception.h"

namespace medPython
{

PythonList::PythonList(PyObject* listObject) :
    TypedObjectHandle(listObject)
{
}

PythonList::PythonList(const PythonObject& other) :
    TypedObjectHandle(other)
{
}

PythonList::PythonList(QList<PythonObject> items) :
    TypedObjectHandle(PyList_New(items.length()))
{
    CHECK_PYTHON_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(data(), i, items[i].newReference());
    }
}

PythonList::PythonList(QList<QString> items) :
    TypedObjectHandle(PyList_New(items.length()))
{
    CHECK_PYTHON_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(data(), i, Convert::toPython(items[i]).newReference());
    }
}

bool PythonList::isEmpty()
{
    return getSize() > 0;
}

int PythonList::getSize()
{
    int size = PyList_Size(data());
    CHECK_PYTHON_ERROR();
    return size;
}

PythonObject PythonList::getItem(int i)
{
    PythonObject item = PythonObject::borrowed(PyList_GetItem(data(), i));
    CHECK_PYTHON_ERROR();
    return item;
}

void PythonList::append(PythonObject item)
{
    PyList_Append(data(), item.newReference());
    CHECK_PYTHON_ERROR();
}

void PythonList::append(QString item)
{
    appendItem(Convert::toPython(item));
}

}
