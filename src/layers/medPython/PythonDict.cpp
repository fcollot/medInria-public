#include "PythonDict.h"

#include <QHashIterator>
#include <QList>

#include "ExceptionManager.h"

namespace medPython
{

PythonDict::PythonDict(PyObject* DictObject) :
    TypeCheckedPythonObject(DictObject)
{
}

PythonDict::PythonDict(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

PythonDict::PythonDict(QHash<PythonObject, PythonObject> items) :
    TypeCheckedPythonObject(PyDict_New())
{
    MEDPYTHON_CHECK_ERROR();

    foreach (PythonObject key, items.keys())
    {
        PyDict_SetItem(data(), key.data(), items[key].data());
        MEDPYTHON_CHECK_ERROR();
    }
}

PythonDict::PythonDict(QHash<QString, PythonObject> items) :
    TypeCheckedPythonObject(PyDict_New())
{
    MEDPYTHON_CHECK_ERROR();

    foreach (QString key, items.keys())
    {
        const char* cString = qUtf8Printable(key);
        PyDict_SetItemString(data(), cString, items[cString].data());
        MEDPYTHON_CHECK_ERROR();
    }
}

int PythonDict::getSize()
{
    int size = PyDict_Size(data());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonDict::getItem(PythonObject key)
{
    PythonObject item = PyDict_GetItemWithError(data(), key.data());
    MEDPYTHON_CHECK_ERROR();

    if (!item)
    {
        throw KeyError::create(MEDPYTHON_CODE_LOCATION, PythonTuple({item}));
    }

    return item;
}

PythonObject PythonDict::getItem(QString key)
{
    return getItem(PythonObject(key));
}

void PythonDict::setItem(PythonObject key, PythonObject value)
{
    PyDict_SetItem(data(), key.data(), value.data());
    MEDPYTHON_CHECK_ERROR();
}

void PythonDict::setItem(QString key, PythonObject value)
{
    PyDict_SetItemString(data(), qUtf8Printable(key), value.data());
    MEDPYTHON_CHECK_ERROR();
}

}
