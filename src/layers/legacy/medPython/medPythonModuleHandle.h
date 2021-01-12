#pragma once

#include <QDir>

#include "medPython.h"
#include "medPythonExport.h"

#define CHECK_MODULE_IS_VALID(handle) if (!((handle).isValid())) THROW_PYTHON_RELATED_EXCEPTION("Not a valid module")

class medPythonModuleHandlePrivate;

class MEDPYTHON_EXPORT medPythonModuleHandle
{
public:
    static bool moduleExists(QString name);

    medPythonModuleHandle(const char* moduleName);
    medPythonModuleHandle(QString moduleName);
    medPythonModuleHandle(PyObject* module = nullptr);
    medPythonModuleHandle(const medPythonModuleHandle& other);
    medPythonModuleHandle& operator=(const medPythonModuleHandle& other);
    ~medPythonModuleHandle();

    bool isValid();
    bool isPackage();

    QString getName();

    PyObject* getModuleObject();

    void runCode(QString pythonCode);

    medPythonModuleHandle createModule(QString name);

    medPythonModuleHandle import(QString name, QStringList fromList = {"*"}, bool relative = false);

    void importObjectsFrom(medPythonModuleHandle other, QStringList objectNames);

private:
    static PyObject* getModuleObject(QString name);

    medPythonModuleHandlePrivate* const d;
};
