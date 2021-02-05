#include "medPythonTuple.h"

#include "medPythonExceptionManager.h"

namespace med
{

int isTupleObject(PyObject* object)
{
    return PyTuple_Check(object);
}

PythonTuple PythonTuple::create(QList<PythonObject> items)
{
    PythonTuple tuple = PyTuple_New(items.length());
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyTuple_SET_ITEM(tuple.self(), i, items[i].newReference());
    }

    return tuple;
}

PythonTuple::PythonTuple(PyObject* tupleObject) :
    TypeCheckedPythonObject(tupleObject)
{
}

PythonTuple::PythonTuple(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

int PythonTuple::getSize()
{
    int size = PyTuple_Size(self());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonTuple::getItem(int i)
{
    PythonObject item = PythonObject::borrowed(PyTuple_GetItem(self(), i));
    MEDPYTHON_CHECK_ERROR();
    return item;
}

void PythonTuple::setItem(int i, PythonObject item)
{
    PyTuple_SetItem(self(), i, item.newReference());
    MEDPYTHON_CHECK_ERROR();
}

}
