#include "PythonTuple.h"

#include <QList>

#include "ExceptionManager.h"

namespace medPython
{

PythonTuple::PythonTuple(PyObject* tupleObject) :
    TypeCheckedPythonObject(tupleObject)
{
}

PythonTuple::PythonTuple(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

PythonTuple::PythonTuple(QList<PythonObject> items) :
    TypeCheckedPythonObject(PyTuple_New(items.length()))
{
    MEDPYTHON_CHECK_ERROR();

    for (int i = 0; i < items.length(); i++)
    {
        PyTuple_SET_ITEM(data(), i, items[i].newReference());
    }
}

int PythonTuple::getSize()
{
    int size = PyTuple_Size(data());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonTuple::getItem(int i)
{
    PythonObject item = PythonObject::borrowed(PyTuple_GetItem(data(), i));
    MEDPYTHON_CHECK_ERROR();
    return item;
}

void PythonTuple::setItem(int i, PythonObject item)
{
    PyTuple_SetItem(data(), i, item.newReference());
    MEDPYTHON_CHECK_ERROR();
}

}
