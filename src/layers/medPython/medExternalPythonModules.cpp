#include "medExternalPythonModules.h"

#include <medSettingsManager.h>

#include "medPythonModule.h"

namespace med
{

void ExternalPythonModules::initialize()
{
    updateSysPath(getPaths());
}

void ExternalPythonModules::setPaths(QStringList paths)
{
    QStringList oldPaths = getPaths();
    medSettingsManager::instance()->setValue("", "python_module_paths", paths);
    updateSysPath(paths, oldPaths);
}

QStringList ExternalPythonModules::getPaths()
{
    return medSettingsManager::instance()->value("", "python_module_paths").toStringList();
}

void ExternalPythonModules::updateSysPath(QStringList newPaths, QStringList oldPaths)
{
    PythonDict imports = PythonModule::import("sys.path");
    PythonList sysPath = imports.getItem("path");

    foreach (QString path, oldPaths)
    {
        sysPath.remove(path);
    }

    foreach (QString path, newPaths)
    {
        sysPath.prepend(path);
    }
}

}
