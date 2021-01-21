#include "PythonModule.h"

#include "ExceptionManager.h"
#include "Resource.h"

namespace medPython
{

PythonModule PythonModule::createModule(QString name)
{
    if (getModule(name))
    {
        PythonObject message("Module " + name + " already exists");
        throw RuntimeError::create(MEDPYTHON_CODE_LOCATION, PythonTuple({message}));
    }

    PythonModule module = PythonModule::borrowed(PyImport_AddModule(qUtf8Printable(name)));
    MEDPYTHON_CHECK_ERROR();
    return module;
}

PythonModule PythonModule::createPackage(QString name)
{
    PythonModule package = createModule(name);
    PythonObject path = PyList_New(2);
    PythonObject resourcePath = Resource::packageNameToResourcePath(package.getName());
    PythonObject libraryPath = qEnvironmentVariable("MEDINRIA_LIBRARY_DIR");
    PyList_SET_ITEM(path.data(), 0, resourcePath.takeReference());
    PyList_SET_ITEM(path.data(), 1, libraryPath.takeReference());
    PyObject_SetAttrString(package.data(), "__path__", path.data());
    MEDPYTHON_CHECK_ERROR();
    return package;
}

PythonModule PythonModule::getModule(QString name)
{
    PythonObject pythonName = name;
    PythonModule module = PyImport_GetModule(pythonName.data());
    MEDPYTHON_CHECK_ERROR();
    return module;
}

PythonModule PythonModule::import(QString name, PythonList fromList)
{
    return internalImport(name, nullptr, fromList);
}

PythonModule PythonModule::importIntoMain(QString name, PythonList fromList)
{
    return getModule("__main__").localImport(name, fromList);
}

PythonModule::PythonModule(PyObject* moduleObject) :
    TypeCheckedPythonObject(moduleObject)
{
}

PythonModule::PythonModule(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

bool PythonModule::isPackage()
{
    return PyObject_HasAttrString(data(), "__path__");
}

QString PythonModule::getName()
{
    QString name = QString(PyModule_GetName(data()));
    MEDPYTHON_CHECK_ERROR();
    return name;
}

PythonDict PythonModule::getDict()
{
    return PythonObject::borrowed(PyModule_GetDict(data()));
}

void PythonModule::runCode(QString pythonCode)
{
    PythonObject globals = PythonObject::borrowed(PyModule_GetDict(data()));
    MEDPYTHON_CHECK_ERROR();

    PythonObject(PyRun_String(qUtf8Printable(pythonCode), Py_file_input, globals.data(), nullptr));
    MEDPYTHON_CHECK_ERROR();
}

PythonModule PythonModule::createSubModule(QString name)
{
    if (!isPackage())
    {
        PythonObject message("Cannot create submodule for " + getName() + " which is not a package");
        throw RuntimeError::create(MEDPYTHON_CODE_LOCATION, PythonTuple({message}));
    }

    QString qualifiedName = getName() + "." + name;
    return createModule(qualifiedName);
}

PythonModule PythonModule::localImport(QString name, PythonList fromList, bool relative)
{
    PythonDict dict = getDict();
    PythonModule importedModule = internalImport(name, dict, fromList, relative);

    if (fromList.isEmpty())
    {
        setAttribute(importedModule.getName(), importedModule);
    }
    else
    {
        PythonDict otherDict = importedModule.getDict();
        int numObjects = fromList.getSize();

        for (int i = 0; i < numObjects; i++)
        {
            QString objectName = fromList.getItem(i).toString();

            if (!objectName.startsWith("_"))
            {
                PythonObject objectValue = otherDict.getItem(objectName);

                if (!objectValue)
                {
                    PythonObject message("Object " + objectName + " not found in module " + importedModule.getName());
                    throw ImportError::create(MEDPYTHON_CODE_LOCATION, PythonTuple({message}));
                }
                else
                {
                    dict.setItem(objectName, objectValue);
                }
            }
        }
    }

    return importedModule;
}

PythonModule PythonModule::internalImport(QString name, PythonDict globals, PythonList fromList, bool relative)
{
    if (fromList.isEmpty())
    {
        fromList.append("*"); // to prevent Python from returning the root package instead of the module
    }

    int level = relative ? 1 : 0;
    PythonModule importedModule = PyImport_ImportModuleLevel(qUtf8Printable(name), globals.data(), nullptr, fromList.data(), level);
    MEDPYTHON_CHECK_ERROR();
    return importedModule;
}

}
