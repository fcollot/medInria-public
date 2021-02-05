#include "medPythonGenericObjectBackend.h"

#include "medPythonExceptionManager.h"

namespace med
{

PythonObject PythonGenericObjectBackend::type(PythonObject& object)
{
    PyObject* result = PyObject_Type(*object);
    return result;
}

QString PythonGenericObjectBackend::typeName(PythonObject& object)
{
    return Py_TYPE(*object)->tp_name;
}

bool PythonGenericObjectBackend::hasAttribute(PythonObject& object, QString name)
{
    return PyObject_HasAttrString(*object, qUtf8Printable(name));
}

PythonObject PythonGenericObjectBackend::getAttribute(PythonObject& object, QString name)
{
    PyObject* result = PyObject_GetAttrString(*object, qUtf8Printable(name));
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

void PythonGenericObjectBackend::setAttribute(PythonObject& object, QString name, PythonObject value)
{
    PyObject_SetAttrString(*object, qUtf8Printable(name), *value);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
}

QList<QString> PythonGenericObjectBackend::dir(PythonObject& object)
{
    PyObject* dir = PyObject_Dir(*object);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    QList<PythonObject> stringsAsObjects = PythonObject(dir).toQList();
    QList<QString> result;

    foreach (const PythonObject& stringObject, stringsAsObjects)
    {
        result.append(stringObject.toString());
    }

    return result;
}

}
