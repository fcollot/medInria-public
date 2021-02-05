#include "medPythonModule.h"

#include "medPythonExceptionManager.h"
#include "medPythonResource.h"

namespace med
{

int isModuleObject(PyObject* object)
{
    return PyModule_Check(object);
}

PythonModule PythonModule::createModule(QString name)
{
    if (get(name))
    {
        PythonObject message = PythonObject::create("Module " + name + " already exists");
        throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
    }

    PythonModule module = PythonModule::borrowed(PyImport_AddModule(qUtf8Printable(name)));
    MEDPYTHON_CHECK_ERROR();
    return module;
}

PythonModule PythonModule::createPackage(QString name)
{
    PythonModule package = createModule(name);
    PythonObject resourcePath = PythonObject::create(PythonResource::packageNameToResourcePath(package.getName()));
    PythonObject libraryPath = PythonObject::create(qEnvironmentVariable("MEDINRIA_LIBRARY_DIR"));
    PythonList path = PythonList::create({resourcePath, libraryPath});
    package.setAttribute("__path__", path);
    return package;
}

PythonModule PythonModule::get(QString name)
{
    PythonObject pythonName = PythonObject::create(name);
    PythonModule module = PyImport_GetModule(*pythonName);
    MEDPYTHON_CHECK_ERROR();
    return module;
}

PythonObject PythonModule::import(QString name, QList<QString> fromList)
{
    PythonModule importedModule = internalImport(name, nullptr, fromList);

    if (fromList.isEmpty())
    {
        return importedModule;
    }
    else
    {
        PythonDict importedObjects = PythonDict::create();

        for (int i = 0; i < fromList.length(); i++)
        {
            QString objectName = fromList[i];
            PythonObject objectValue = importedModule.getAttribute(objectName);
            importedObjects.setItem(objectName, objectValue);
        }

        return importedObjects;
    }
}

PythonModule PythonModule::importIntoMain(QString name, QList<QString> fromList)
{
    return get("__main__").localImport(name, fromList);
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

}

QString PythonModule::getName()
{
    QString name = QString(PyModule_GetName(self()));
    MEDPYTHON_CHECK_ERROR();
    return name;
}

PythonDict PythonModule::getDict()
{
    PythonDict dict = PythonObject::borrowed(PyModule_GetDict(self()));
    MEDPYTHON_CHECK_ERROR();
    return dict;
}

void PythonModule::runCode(QString pythonCode)
{
    PythonObject(PyRun_String(qUtf8Printable(pythonCode), Py_file_input, *getDict(), nullptr));
    MEDPYTHON_CHECK_ERROR();
}

PythonModule PythonModule::createSubModule(QString name)
{
    if (!isPackage())
    {
        PythonObject message = PythonObject::create("Cannot create submodule for " + getName() + " which is not a package");
        throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
    }

    QString qualifiedName = getName() + "." + name;
    return createModule(qualifiedName);
}

PythonModule PythonModule::localImport(QString name, QList<QString> fromList, bool relative)
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

        for (int i = 0; i < fromList.length(); i++)
        {
            QString objectName = fromList[i];

            if (!objectName.startsWith("_"))
            {
                PythonObject objectValue = otherDict.getItem(objectName);

                if (!objectValue)
                {
                    PythonObject message = PythonObject::create("Object " + objectName + " not found in module " + importedModule.getName());
                    throw PythonImportError::create(MEDPYTHON_CODE_LOCATION, {message});
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

PythonModule PythonModule::internalImport(QString name, PythonDict globals, QList<QString> fromList, bool relative)
{
    if (fromList.isEmpty())
    {
        fromList.append("*"); // to prevent Python from returning the root package instead of the module
    }

    int level = relative ? 1 : 0;
    PythonList fromList_python = PythonList::create(fromList);
    PythonModule importedModule = PyImport_ImportModuleLevel(qUtf8Printable(name), *globals, nullptr, *fromList_python, level);
    MEDPYTHON_CHECK_ERROR();
    return importedModule;
}

}
