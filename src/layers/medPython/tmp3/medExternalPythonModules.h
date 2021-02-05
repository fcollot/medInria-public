#pragma once

#include <QStringList>

#include "medPythonExport.h"

namespace med
{

class MEDPYTHON_EXPORT ExternalPythonModules
{
public:
    static void initialize();

    static void setPaths(QStringList paths);
    static QStringList getPaths();

private:
    static void updateSysPath(QStringList newPaths, QStringList oldPaths = QStringList());

    ExternalPythonModules() = default;
    ~ExternalPythonModules() = default;
};

}
