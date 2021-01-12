#include "medPythonModuleHandle.h"

#include "medPythonError.h"
#include "medPythonModuleFactory.h"
#include "medPythonTypeUtils.h"

class medPythonModuleHandlePrivate
{
public:
    PyObject* module;
};

bool medPythonModuleHandle::moduleExists(QString name)
{
    return (getModuleObject(name) != nullptr);
}

medPythonModuleHandle::medPythonModuleHandle(const char* moduleName) :
    d(new medPythonModuleHandlePrivate)
{
    *this = medPythonModuleHandle(QString(moduleName));
}

medPythonModuleHandle::medPythonModuleHandle(QString moduleName) :
    d(new medPythonModuleHandlePrivate)
{
    PyObject* module = getModuleObject(moduleName);
    Py_XINCREF(module);
    d->module = module;
}

medPythonModuleHandle::medPythonModuleHandle(PyObject* module) :
    d(new medPythonModuleHandlePrivate)
{
    if (module && !PyModule_Check(module))
    {
        THROW_PYTHON_RELATED_EXCEPTION("Object of type " + Py_TYPE(module)->tp_name + " is not a python module");
    }

    d->module = module;
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

bool medPythonModuleHandle::isPackage()
{
    return isValid() && PyObject_HasAttrString(d->module, "__path__");
}

QString medPythonModuleHandle::getName()
{
    CHECK_MODULE_IS_VALID(*this);

    QString name = QString(PyModule_GetName(d->module));
    CHECK_PYTHON_ERROR();
    return name;
}

PyObject* medPythonModuleHandle::getModuleObject()
{
    return d->module;
}

void medPythonModuleHandle::runCode(QString pythonCode)
{
    CHECK_MODULE_IS_VALID(*this);

    PyObject* moduleDict = PyModule_GetDict(d->module);
    CHECK_PYTHON_ERROR();

    medPythonUtils::runCode(pythonCode, moduleDict);
}

medPythonModuleHandle medPythonModuleHandle::createModule(QString name)
{
    if (!isPackage())
    {
        THROW_PYTHON_RELATED_EXCEPTION("Not a valid package");
    }

    QString qualifiedName = getName() + "." + name;
    return medPythonModuleFactory().createModule(qualifiedName);
}

medPythonModuleHandle medPythonModuleHandle::import(QString name, QStringList fromList, bool relative)
{
    return medPythonModuleFactory().import(name, *this, fromList, relative);
}

void medPythonModuleHandle::importObjectsFrom(medPythonModuleHandle other, QStringList objectNames)
{
    CHECK_MODULE_IS_VALID(*this);

    PyObject* dict = PyModule_GetDict(getModuleObject());
    CHECK_PYTHON_ERROR();

    PyObject* otherDict = PyModule_GetDict(other.getModuleObject());
    CHECK_PYTHON_ERROR();

    PyObject* objectNames_python;

    if ((objectNames.length() == 1 && objectNames.first() == "*"))
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
                THROW_PYTHON_RELATED_EXCEPTION("Object " + objectNames[i] + " not found in module " + other.getName());
            }
            else
            {
                PyDict_SetItem(dict, objectName, objectValue);
            }
        }
    }

    Py_DECREF(objectNames_python);
}

PyObject* medPythonModuleHandle::getModuleObject(QString name)
{
    PyObject* pythonName = medPythonTypeUtils::toPython(name);
    PyObject* module = PyImport_GetModule(pythonName);
    Py_XDECREF(module);
    Py_DECREF(pythonName);
    CHECK_PYTHON_ERROR();
    return module;
}
