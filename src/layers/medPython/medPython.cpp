#include "medPython.h"

namespace med
{
namespace python
{

PythonObject Python::import(QString name, QList<QString> fromList)
{
    return PythonModuleBackend::import(name, fromList);
}

PythonObject Python::createModule(QString name)
{
    return PythonModuleBackend::createModule(name);
}

PythonObject Python::createPackage(QString name)
{
    return PythonModuleBackend::createPackage(name);
}

PythonObject Python::getModule(QString name)
{
    return PythonModuleBackend::getModule(name);
}

PythonObject Python::tuple(QList<PythonObject> items)
{
    return PythonContainerBackend::createSequenceFromQList<PythonObject, &PyTuple_New>(items);
}

PythonObject Python::tuple(QList<QString> items)
{
    return PythonContainerBackend::createSequenceFromQList<QString, &PyTuple_New>(items);
}

PythonObject Python::tuple(QList<long> items)
{
    return PythonContainerBackend::createSequenceFromQList<long, &PyTuple_New>(items);
}

PythonObject Python::tuple(QList<double> items)
{
    return PythonContainerBackend::createSequenceFromQList<double, &PyTuple_New>(items);
}

PythonObject Python::tuple(PythonObject& object)
{
    return PythonWrappers::functionReturnObject<&PySequence_Tuple>(object);
}

PythonObject Python::list(QList<PythonObject> items)
{

}

PythonObject Python::list(PythonObject& object)
{
    return PythonWrappers::functionReturnObject<&PySequence_List>(object);
}

PythonObject Python::dict(QHash<PythonObject, PythonObject> items)
{
    PythonObject dict = PythonWrappers::creatorFunction<&PyDict_New>();

    foreach (const PythonObject& key, items.keys())
    {
        PythonWrappers::functionNoReturn<&PyDict_SetItem, PyObject*, PyObject*>(dict, *key, *items[key]);
    }

    return dict;
}

PythonObject Python::none()
{
    return borrow(Py_None);
}

size_t Python::len(PythonObject& object)
{
    return object.length();
}

QList<QString> Python::dir(PythonObject& object)
{
    return object.dir();
}

PythonObject Python::take(PyObject* nativeObject)
{
    return PythonObject::take(nativeObject);
}

PythonObject Python::borrow(PyObject* nativeObject)
{
    return PythonObject::borrow(nativeObject);
}

}
