#include "medPythonModuleFunctions.h"

#include "medPythonContainerConversions.h"
#include "medPythonExceptions.h"
#include "medPythonGeneralFunctions.h"
#include "medPythonPrimitiveConversions.h"
#include "medPythonResource.h"

namespace med
{
namespace python
{
namespace internal
{

bool isModule(const AbstractReference& object)
{
    return PyModule_Check(object.get());
}

bool isPackage(const AbstractReference& object)
{
    return isModule(object) && hasAttribute(object, "__path__");
}

Reference createModule(QString name)
{
    if (getModule(name))
    {
        throw RuntimeError(MEDPYTHON_CODE_LOCATION, QString("Module %1 already exists").arg(name));
    }

    return Reference::borrowed(coreFunction<PyObject*, &PyImport_AddModule, const char*>(qUtf8Printable(name)));
}

Reference createPackage(QString name)
{
    Reference package = createModule(name);
    Reference resourcePath = Convert<QString>::toPython(PythonResource::packageNameToResourcePath(moduleName(package)));
    Reference libraryPath = Convert<QString>::toPython(qEnvironmentVariable("MEDINRIA_LIBRARY_DIR"));
    Reference path = Convert<QList<Reference> >::toPython({resourcePath, libraryPath});
    setAttribute(package, "__path__", path);
    return package;
}

Reference createSubModule(const AbstractReference& package, QString name)
{
    if (!isPackage(package))
    {
        throw TypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a package").arg(getTypeName(package)));
    }

    QString qualifiedName = moduleName(package) + "." + name;
    return createModule(qualifiedName);
}

Reference getModule(QString name)
{
    Reference pythonName = Convert<QString>::toPython(name);
    Reference result = coreFunction<PyObject*, &PyImport_GetModule, PyObject*>(pythonName.get());
    return result ? result : none();
}

void runCodeInModuleContext(const AbstractReference& module, QString pythonCode)
{
    if (!isModule(module))
    {
        throw TypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a module").arg(getTypeName(module)));
    }

    Reference dict = moduleDict(module);
    PyObject* result = coreFunction<PyObject*, &PyRun_String, const char*, int , PyObject*, PyObject*>
            (qUtf8Printable(pythonCode), Py_file_input, dict.get(), nullptr);
    Py_XDECREF(result);
}

QString moduleName(const AbstractReference& module)
{
    return coreFunction<const char*, &PyModule_GetName, PyObject*>(module.get());
}

Reference moduleDict(const AbstractReference& module)
{
    return Reference::borrowed(coreFunction<PyObject*, &PyModule_GetDict, PyObject*>(module.get()));
}

Reference import(QString name, QList<QString> fromList, const AbstractReference& globals, bool relative)
{
    Reference fromList_python = Convert<QList<QString> >::toPython(fromList);
    Reference importedModule = coreFunction<PyObject*, &PyImport_ImportModuleLevel, const char*, PyObject*, PyObject*, PyObject*, int>
            (qUtf8Printable(name), globals.get(), nullptr, fromList_python.get(), relative);

    if (fromList.isEmpty())
    {
        return importedModule;
    }
    else
    {
        return retrieveModuleObjects(importedModule, fromList);
    }
}

Reference import(const AbstractReference& importingModule, QString name, QList<QString> fromList, bool relative)
{
    Reference dict = moduleDict(importingModule);
    Reference importResult = import(name, fromList, dict, relative);

    if (PyDict_Check(importResult.get()))
    {
        mergeDicts(dict, importResult);
    }

    return importResult;
}

Reference retrieveModuleObjects(const AbstractReference& sourceModule, QList<QString> fromList)
{
    if ((fromList.length() == 1) && (fromList[0] == "*"))
    {
        fromList = publicModuleAttributes(sourceModule);
    }

    Reference moduleObjects = createDict();
    size_t numObjects = fromList.length();

    for (size_t i = 0; i < numObjects; i++)
    {
        QString objectName = fromList[i];
        setItem(moduleObjects, objectName, getAttribute(sourceModule, objectName));
    }
}

QList<QString> publicModuleAttributes(const AbstractReference& module)
{
    QList<QString> result;
    Reference dict = moduleDict(module);
    Reference keys = mappingKeys(dict);
    size_t numKeys = getSize(keys);

    for (size_t i = 0; i < numKeys; i++)
    {
        QString objectName = Convert<QString>::fromPython(getItem(keys, i));

        if (!objectName.startsWith("_")) // names starting with underscore are considered private in Python
        {
            result.append(objectName);
        }
    }

    return result;
}

}
}
}
