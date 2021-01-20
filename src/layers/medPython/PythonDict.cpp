#include "PythonDict.h"

#include <QHashIterator>
#include <QList>

#include "Convert.h"
#include "Exception.h"

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
    CHECK_PYTHON_ERROR();

    foreach (PythonObject key, items.keys())
    {
        PyDict_SetItem(data(), key.data(), items[key].data());
        CHECK_PYTHON_ERROR();
    }
}

PythonDict::PythonDict(QHash<QString, PythonObject> items) :
    TypeCheckedPythonObject(PyDict_New())
{
    CHECK_PYTHON_ERROR();

    foreach (QString key, items.keys())
    {
        const char* cString = TO_CSTRING(key);
        PyDict_SetItemString(data(), cString, items[cString].data());
        CHECK_PYTHON_ERROR();
    }
}

int PythonDict::getSize()
{
    int size = PyDict_Size(data());
    CHECK_PYTHON_ERROR();
    return size;
}

PythonObject PythonDict::getItem(PythonObject key)
{
    PythonObject item = PyDict_GetItemWithError(data(), key.data());
    CHECK_PYTHON_ERROR();

    if (!item)
    {

        Error.convertAndThrow();
    }

    return item;
}

PythonObject PythonDict::getItem(QString key)
{
    return getItem(Convert::toPython(key));
}

void PythonDict::setItem(PythonObject key, PythonObject value)
{
    PyDict_SetItem(data(), key.data(), value.data());
    CHECK_PYTHON_ERROR();
}

void PythonDict::setItem(QString key, PythonObject value)
{
    PyDict_SetItemString(data(), TO_CSTRING(key), value.data());
    CHECK_PYTHON_ERROR();
}

}
