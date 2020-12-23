#pragma once

#include <QDir>

#include "medPythonExport.h"
#include "medPythonUtils.h"

class medPythonModuleManagerPrivate;

class MEDPYTHON_EXPORT medPythonModuleManager
{
public:
    static medPythonModuleManager* instance();
    static void initialize();

    QString createTemporaryModuleFromString(QString moduleCode, QString moduleName = "");

    void extractResourceModule(QString resourceLocation, QString moduleName);

    PyObject* loadModule(QString moduleName, bool reload = false);

    ~medPythonModuleManager();

private:
    medPythonModuleManager();

    medPythonModuleManagerPrivate* const d;
};
