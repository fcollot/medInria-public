#include "medPythonModuleFactory.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "medPythonErrorChecker.h"
#include "medPythonTypeUtils.h"

bool medPythonModuleFactory::moduleExists(QString name)
{
    return (getModuleObject(name) != nullptr);
}

medPythonModuleHandle medPythonModuleFactory::getModule(QString name)
{
    return medPythonModuleHandle(getModuleObject(name));
}

medPythonModuleHandle medPythonModuleFactory::createModule(QString name)
{
    if (moduleExists(name))
    {
        THROW_EXCEPTION("Module " + name + " already exists");
    }

    PyObject* module = PyImport_AddModule(qUtf8Printable(name));
    CHECK_PYTHON_ERROR();
    return medPythonModuleHandle(module);
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
    return package;
}

QString medPythonModuleFactory::getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
        THROW_EXCEPTION("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
    }

    QString sourceCode;
    QTextStream stream(&resourceFile);
    sourceCode = stream.readAll();
    resourceFile.close();
    return sourceCode;
}

PyObject* medPythonModuleFactory::getModuleObject(QString name)
{
    PyObject* pythonName = medPythonTypeUtils::toPython(name);
    PyObject* module = PyImport_GetModule(pythonName);
    Py_XDECREF(module);
    Py_DECREF(pythonName);
    TEST_PYTHON_ERROR;
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
