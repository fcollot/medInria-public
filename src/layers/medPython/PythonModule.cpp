#include "PythonModule.h"

#include "Convert.h"
#include "Exception.h"

namespace medPython
{

PythonModule PythonModule::createModule(QString name)
{
    if (getModule(name))
    {
        THROW_PYTHON_RELATED_EXCEPTION("Module " + name + " already exists");
    }

    PythonModule module = PythonModule::borrowed(PyImport_AddModule(TO_CSTRING(name)));
    CHECK_PYTHON_ERROR();
    return module;
}

PythonModule PythonModule::createPackage(QString name)
{
    PythonModule package = createModule(name);
    PythonObject path = PyList_New(2);
    PythonObject resourcePath = Convert::toPython(packageNameToResourcePath(package.getName()));
    PythonObject libraryPath = Convert::toPython(qEnvironmentVariable("MEDINRIA_LIBRARY_DIR"));
    PyList_SET_ITEM(path.data(), 0, resourcePath.takeReference());
    PyList_SET_ITEM(path.data(), 1, libraryPath.takeReference());
    PyObject_SetAttrString(package.data(), "__path__", path.data());
    CHECK_PYTHON_ERROR();
    return package;
}

PythonModule PythonModule::getModule(QString name)
{
    PythonObject pythonName = Convert::toPython(name);
    PythonModule module = PyImport_GetModule(pythonName.data());
    CHECK_PYTHON_ERROR();
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
    CHECK_PYTHON_ERROR();
    return name;
}

PythonDict PythonModule::getDict()
{
    return PythonObject::borrowed(PyModule_GetDict(data()));
}

void PythonModule::runCode(QString pythonCode)
{
    PythonObject globals = PythonObject::borrowed(PyModule_GetDict(data()));
    CHECK_PYTHON_ERROR();

    PythonObject(PyRun_String(TO_CSTRING(pythonCode), Py_file_input, globals.data(), nullptr));
    CHECK_PYTHON_ERROR();
}

PythonModule PythonModule::createSubModule(QString name)
{
    if (!isPackage())
    {
        THROW_PYTHON_RELATED_EXCEPTION("Not a package");
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
            QString objectName = Convert::fromString(fromList.getItem(i));

            if (!objectName.startsWith("_"))
            {
                PythonObject objectValue = otherDict.getItem(objectName);

                if (!objectValue)
                {
                    THROW_PYTHON_RELATED_EXCEPTION("Object " + objectName + " not found in module " + importedModule.getName());
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
    PythonModule importedModule = PyImport_ImportModuleLevel(TO_CSTRING(name), globals.data(), nullptr, fromList.data(), level);
    CHECK_PYTHON_ERROR();
    return importedModule;
}

}
