#pragma once

#include "medPythonExport.h"
#include "medPythonModuleHandle.h"

class MEDPYTHON_EXPORT medPythonModuleFactory
{
public:
    medPythonModuleHandle createModule(QString name);
    medPythonModuleHandle createPackage(QString name);

    QString getResourceModuleSourceCode(QString name);

    medPythonModuleHandle import(QString name, medPythonModuleHandle destModule = "__main__", QStringList fromList = {"*"}, bool relative = false);

private:
    QString moduleNameToResourcePath(QString name);
    QString packageNameToResourcePath(QString name);
};
