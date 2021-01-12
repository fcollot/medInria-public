#include "medPythonModuleFactory.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "medPythonError.h"
#include "medPythonTypeUtils.h"

medPythonModuleHandle medPythonModuleFactory::createModule(QString name)
{
    if (medPythonModuleHandle::moduleExists(name))
    {
        THROW_PYTHON_RELATED_EXCEPTION("Module " + name + " already exists");
    }

    PyObject* module = PyImport_AddModule(TO_CSTRING(name));
    CHECK_PYTHON_ERROR();
    Py_INCREF(module);
    return module;
}

medPythonModuleHandle medPythonModuleFactory::createPackage(QString name)
{
    medPythonModuleHandle package = createModule(name);
    PyObject* path = PyList_New(2);
    PyObject* resourcePath = medPythonTypeUtils::toPython(packageNameToResourcePath(package.getName()));
    PyObject* libraryPath = medPythonTypeUtils::toPython(qEnvironmentVariable("MEDINRIA_LIBRARY_DIR"));
    PyList_SET_ITEM(path, 0, resourcePath);
    PyList_SET_ITEM(path, 1, libraryPath);
    PyObject_SetAttrString(package.getModuleObject(), "__path__", path);
    Py_DECREF(path);
    CHECK_PYTHON_ERROR();
    return package;
}

QString medPythonModuleFactory::getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
        THROW_PYTHON_RELATED_EXCEPTION("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
    }

    QString sourceCode;
    QTextStream stream(&resourceFile);
    sourceCode = stream.readAll();
    resourceFile.close();
    return sourceCode;
}

medPythonModuleHandle medPythonModuleFactory::import(QString name, medPythonModuleHandle destModule, QStringList fromList, bool relative)
{
    CHECK_MODULE_IS_VALID(destModule);

    int level = relative ? 1 : 0;
    PyObject* fromList_python = medPythonTypeUtils::toPython(fromList);
    PyObject* module = PyImport_ImportModuleLevel(TO_CSTRING(name), destModule.getModuleObject(), nullptr, fromList_python, level);
    Py_XDECREF(fromList_python);
    CHECK_PYTHON_ERROR();

    if (fromList.isEmpty())
    {
        PyObject_SetAttrString(destModule.getModuleObject(), PyModule_GetName(module), module);
    }
    else
    {
        destModule.importObjectsFrom(module, fromList);
    }

    return module;
}

QString medPythonModuleFactory::moduleNameToResourcePath(QString name)
{
    return packageNameToResourcePath(name).append(".py");
}

QString medPythonModuleFactory::packageNameToResourcePath(QString name)
{
    return name.prepend(":/python/").replace('.', "/");
}
