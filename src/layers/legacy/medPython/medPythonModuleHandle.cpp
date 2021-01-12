#include "medPythonModuleHandle.h"

#include "medPythonErrorChecker.h"
#include "medPythonModuleFactory.h"
#include "medPythonTypeUtils.h"

#define TEST_IF_VALID_HANDLE if (!isValid()) THROW_EXCEPTION("Not a valid module")

class medPythonModuleHandlePrivate
{
public:
    PyObject* module;
    medPythonErrorChecker errorChecker;
};

medPythonModuleHandle::medPythonModuleHandle(PyObject* module) :
    d(new medPythonModuleHandlePrivate)
{
    if (module && !PyModule_Check(module))
    {
        THROW_EXCEPTION("Object of type " + Py_TYPE(module)->tp_name + " is not a python module");
    }

    d->module = module;
    Py_XINCREF(d->module);
}

medPythonModuleHandle::medPythonModuleHandle(const medPythonModuleHandle& other) :
    d(new medPythonModuleHandlePrivate)
{
    d->module = other.d->module;
    Py_XINCREF(d->module);
}

medPythonModuleHandle& medPythonModuleHandle::operator=(const medPythonModuleHandle& other)
{
    Py_XDECREF(d->module);
    d->module = other.d->module;
    Py_XINCREF(d->module);
    return *this;
}

medPythonModuleHandle::~medPythonModuleHandle()
{
    Py_XDECREF(d->module);
    delete d;
}

bool medPythonModuleHandle::isValid()
{
    return (d->module != nullptr);
}

QString medPythonModuleHandle::getName()
{
    TEST_IF_VALID_HANDLE;

    QString name = QString(PyModule_GetName(d->module));
    CHECK_PYTHON_ERROR(d->errorChecker);
    return name;
}

PyObject* medPythonModuleHandle::getModuleObject()
{
    return d->module;
}

void medPythonModuleHandle::runString(QString sourceCode)
{
    TEST_IF_VALID_HANDLE;

    PyObject* moduleDict = PyModule_GetDict(d->module);
    CHECK_PYTHON_ERROR(d->errorChecker);

    medPythonUtils::runPythonString(sourceCode, moduleDict);
}

medPythonModuleHandle medPythonModuleHandle::import(QString name, QStringList fromList, bool relative)
{
    TEST_IF_VALID_HANDLE;

    medPythonModuleHandle handle = medPythonModuleFactory().getModule(name);

    if (!handle.isValid())
    {
        PyObject* globals = PyModule_GetDict(d->module);
        CHECK_PYTHON_ERROR(d->errorChecker);

        int level = relative ? 1 : 0;
        PyObject* fromList_python = medPythonTypeUtils::toPython(fromList);
        PyObject* module = PyImport_ImportModuleLevel(qUtf8Printable(name), globals, nullptr, fromList_python, level);
        Py_XDECREF(module);
        Py_XDECREF(fromList_python);
        CHECK_PYTHON_ERROR(d->errorChecker);

        handle = medPythonModuleHandle(module);
    }

    return handle;
}

void medPythonModuleHandle::importObjectsFrom(medPythonModuleHandle& other, QStringList objectNames)
{
    TEST_IF_VALID_HANDLE;

    PyObject* dict = PyModule_GetDict(getModuleObject());
    CHECK_PYTHON_ERROR(d->errorChecker);

    PyObject* otherDict = PyModule_GetDict(other.getModuleObject());
    CHECK_PYTHON_ERROR(d->errorChecker);

    PyObject* objectNames_python;

    if (objectNames.isEmpty())
    {
        objectNames_python = PyDict_Keys(otherDict);
    }
    else
    {
        objectNames_python = medPythonTypeUtils::toPython(objectNames);
    }

    int numObjects = PyList_GET_SIZE(objectNames_python);

    for (int i = 0; i < numObjects; i++)
    {
        PyObject* objectName = PyList_GET_ITEM(objectNames_python, i);

        if (!medPythonTypeUtils::fromString(objectName).startsWith("_"))
        {
            PyObject* objectValue = PyDict_GetItem(otherDict, objectName);

            if (!objectValue)
            {
                Py_DECREF(objectNames_python);
                THROW_EXCEPTION("Object " + objectNames[i] + " not found in module " + other.getName());
            }
            else
            {
                PyDict_SetItem(dict, objectName, objectValue);
            }
        }
    }

    Py_DECREF(objectNames_python);
}
