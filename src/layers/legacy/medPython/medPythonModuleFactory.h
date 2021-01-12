#pragma once

#include "medPythonExport.h"
#include "medPythonModuleHandle.h"
#include "medPythonUtils.h"

class MEDPYTHON_EXPORT medPythonModuleFactory
{
public:
    bool moduleExists(QString name);
    medPythonModuleHandle getModule(QString name);

    medPythonModuleHandle createModule(QString name);
    medPythonModuleHandle createPackage(QString name);

    QString getResourceModuleSourceCode(QString name);

private:
    PyObject* getModuleObject(QString name);

    QString moduleNameToResourcePath(QString name);
    QString packageNameToResourcePath(QString name);
};
