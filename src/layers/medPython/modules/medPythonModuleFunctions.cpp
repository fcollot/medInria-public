/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonModuleFunctions.h"

#include "medPythonContainerConversions.h"
#include "medPythonCoreWrappers.h"
#include "medPythonExceptions.h"
#include "medPythonPrimitiveConversions.h"
#include "medPythonResource.h"

namespace med::python
{

bool isModule(const Object& object)
{
    return PyModule_Check(*object);
}

bool isPackage(const Object& object)
{
    return isModule(object) && object.hasAttribute("__path__");
}

Object createModule(QString name)
{
    if (!getModule(name))
    {
        throw RuntimeError(MEDPYTHON_CODE_LOCATION, QString("Module %1 already exists").arg(name));
    }

    return Object::borrowed(coreFunction<PyObject*, &PyImport_AddModule, const char*>(qUtf8Printable(name)));
}

Object createPackage(QString name)
{
    Object package = createModule(name);
    Object resourcePath = string(packageNameToResourcePath(moduleName(package)));
    Object libraryPath = string(qEnvironmentVariable("MEDINRIA_LIBRARY_DIR"));
    Object path = list<Object>({resourcePath, libraryPath});
    package.setAttribute("__path__", path);
    return package;
}

Object createSubModule(const Object& package, QString name)
{
    if (!isPackage(package))
    {
        throw TypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a package").arg(package.typeName()));
    }

    QString qualifiedName = moduleName(package) + "." + name;
    return createModule(qualifiedName);
}

Object getModule(QString name)
{
    Object pythonName = string(name);
    return coreFunction<PyObject*, &PyImport_GetModule, PyObject*>(*pythonName);
}

void runCodeInModuleContext(const Object& module, QString pythonCode)
{
    if (!isModule(module))
    {
        throw TypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a module").arg(module.typeName()));
    }

    Object dict = moduleDict(module);
    PyObject* result = coreFunction<PyObject*, &PyRun_String, const char*, int , PyObject*, PyObject*>
            (qUtf8Printable(pythonCode), Py_file_input, *dict, nullptr);
    Py_CLEAR(result);
}

QString moduleName(const Object& module)
{
    return coreFunction<const char*, &PyModule_GetName, PyObject*>(*module);
}

Object moduleDict(const Object& module)
{
    return Object::borrowed(coreFunction<PyObject*, &PyModule_GetDict, PyObject*>(*module));
}

Object import(QString name, QList<QString> fromList, const Object& globals, bool relative)
{
    Object fromList_python = list(fromList);
    Object importedModule = coreFunction<PyObject*, &PyImport_ImportModuleLevel, const char*, PyObject*, PyObject*, PyObject*, int>
            (qUtf8Printable(name), *globals, nullptr, *fromList_python, relative);

    if (fromList.isEmpty())
    {
        return importedModule;
    }
    else
    {
        return retrieveModuleObjects(importedModule, fromList);
    }
}

Object import(const Object& importingModule, QString name, QList<QString> fromList, bool relative)
{
    Object dict = moduleDict(importingModule);
    Object importResult = import(name, fromList, dict, relative);

    if (PyDict_Check(*importResult))
    {
        dict.update(importResult);
    }

    return importResult;
}

Object retrieveModuleObjects(const Object& sourceModule, QList<QString> fromList)
{
    if ((fromList.length() == 1) && (fromList[0] == "*"))
    {
        fromList = publicModuleAttributes(sourceModule);
    }

    Object moduleObjects = dict();
    ssize_t numObjects = fromList.length();

    for (ssize_t i = 0; i < numObjects; i++)
    {
        QString objectName = fromList[i];
        moduleObjects[string(objectName)] = sourceModule.getAttribute(objectName);
    }

    return moduleObjects;
}

QList<QString> publicModuleAttributes(const Object& module)
{
    QList<QString> result;
    Object dict = moduleDict(module);
    Object keys = dict.keys();
    ssize_t numKeys = keys.length();

    for (ssize_t i = 0; i < numKeys; i++)
    {
        QString objectName = keys[i].convert<QString>();

        if (!objectName.startsWith("_")) // names starting with underscore are considered private in Python
        {
            result.append(objectName);
        }
    }

    return result;
}

} // namespace med::python
