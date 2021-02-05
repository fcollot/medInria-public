#include "medPythonDict.h"

#include <QHashIterator>
#include <QList>

#include "medPythonExceptionManager.h"

namespace med
{

int isDictObject(PyObject* object)
{
    return PyDict_Check(object);
}

PythonDict PythonDict::create(QHash<PythonObject, PythonObject> items)
{
    PythonDict dict = PyDict_New();
    MEDPYTHON_CHECK_ERROR();

    foreach (PythonObject key, items.keys())
    {
        PyDict_SetItem(*dict, *key, *items[key]);
        MEDPYTHON_CHECK_ERROR();
    }

    return dict;
}

PythonDict PythonDict::create(QHash<QString, PythonObject> items)
{
    PythonDict dict = PyDict_New();
    MEDPYTHON_CHECK_ERROR();

    foreach (QString key, items.keys())
    {
        const char* cString = qUtf8Printable(key);
        PyDict_SetItemString(*dict, cString, *items[cString]);
        MEDPYTHON_CHECK_ERROR();
    }

    return dict;
}

PythonDict::PythonDict(PyObject* DictObject) :
    TypeCheckedPythonObject(DictObject)
{
}

PythonDict::PythonDict(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

int PythonDict::getSize()
{
    int size = PyDict_Size(self());
    MEDPYTHON_CHECK_ERROR();
    return size;
}

PythonObject PythonDict::getItem(PythonObject key)
{
    PythonObject item = PyDict_GetItemWithError(self(), *key);
    MEDPYTHON_CHECK_ERROR();

    if (!item)
    {
        throw PythonKeyError::create(MEDPYTHON_CODE_LOCATION, {item});
    }

    return item;
}

PythonObject PythonDict::getItem(QString key)
{
    return getItem(PythonObject::create(key));
}

void PythonDict::setItem(PythonObject key, PythonObject value)
{
    PyDict_SetItem(self(), *key, *value);
    MEDPYTHON_CHECK_ERROR();
}

void PythonDict::setItem(QString key, PythonObject value)
{
    PyDict_SetItemString(self(), qUtf8Printable(key), *value);
    MEDPYTHON_CHECK_ERROR();
}

}
