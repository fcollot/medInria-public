#include "medPythonList.h"

#include <QList>

#include "medPythonExceptionManager.h"

namespace med
{

int isPythonListObject(PyObject* object)
{
    return PyList_Check(object);
}

PythonList PythonList::create(QList<PythonObject> items)
{
    PythonList list = PyList_New(items.length());
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(*list, i, items[i].newReference());
    }

    return list;
}

PythonList PythonList::create(QList<QString> items)
{
    PythonList list = PyList_New(items.length());
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyList_SET_ITEM(*list, i, PythonObject::create(items[i]).newReference());
    }

    return list;
}

PythonList::PythonList(PyObject* listObject) :
    TypeCheckedPythonObject(listObject)
{
}

PythonList::PythonList(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

bool PythonList::isEmpty()
{
    return getSize() > 0;
}

int PythonList::getSize()
{
    int size = PyList_Size(self());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonList::getItem(int i)
{
    PythonObject item = PythonObject::borrowed(PyList_GetItem(self(), i));
    MEDPYTHON_CHECK_ERROR();
    return item;
}

void PythonList::append(PythonObject item)
{
    PyList_Append(self(), item.newReference());
    MEDPYTHON_CHECK_ERROR();
}

void PythonList::append(QString item)
{
    append(PythonObject::create(item));
}

void PythonList::prepend(PythonObject item)
{
    PyList_Insert(self(), 0, item.newReference());
    MEDPYTHON_CHECK_ERROR();
}

void PythonList::prepend(QString item)
{
    prepend(PythonObject::create(item));
}

void PythonList::remove(PythonObject itemToRemove)
{
    PythonList newList = PythonList::create();
    PythonObject iterator = PyObject_GetIter(self());
    MEDPYTHON_CHECK_ERROR();
    PythonObject item;
    bool found = false;

    while ((item = PyIter_Next(*iterator)))
    {
        if (item == itemToRemove)
        {
            found = true;
        }
        else
        {
            newList.append(item);
        }
    }

    if (found)
    {
        *this = newList;
    }
    else
    {
        PythonObject message = PythonObject::create("List does not contain " + itemToRemove.toString());
        throw PythonValueError::create(MEDPYTHON_CODE_LOCATION, {message});
    }
}

void PythonList::remove(QString item)
{
    remove(PythonObject::create(item));
}

}
