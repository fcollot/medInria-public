#pragma once

#include <QStringList>

#include "medPythonExport.h"

class MEDPYTHON_EXPORT medPythonModulePaths
{
public:
    static void setPaths(QStringList paths);
    static QStringList getPaths();

private:
    static void updateSysPath(QStringList newPaths, QStringList oldPaths = QStringList());

    medPythonModulePaths() = default;
    ~medPythonModulePaths() = default;
};
