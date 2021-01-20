#pragma once

#include <QStringList>

#include "Export.h"

namespace medPython
{

class MEDPYTHON_EXPORT ModulePaths
{
public:
    static void setPaths(QStringList paths);
    static QStringList getPaths();

private:
    static void updateSysPath(QStringList newPaths, QStringList oldPaths = QStringList());

    ModulePaths() = default;
    ~ModulePaths() = default;
};

}
