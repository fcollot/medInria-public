#include "medPythonModulePaths.h"

#include <medSettingsManager.h>

#include "medPythonUtils.h"

void medPythonModulePaths::initialize()
{
    updateSysPath(getPaths());
}

void medPythonModulePaths::setPaths(QStringList paths)
{
    QStringList oldPaths = getPaths();
    medSettingsManager::instance()->setValue("medPython", "python_module_paths", paths);
    updateSysPath(paths, oldPaths);
}

QStringList medPythonModulePaths::getPaths()
{
    return medSettingsManager::instance()->value("medPython", "python_module_paths").toStringList();
}

void medPythonModulePaths::updateSysPath(QStringList newPaths, QStringList oldPaths)
{
    foreach (QString path, oldPaths)
    {
        medPythonUtils::removeSysPath(path);
    }

    foreach (QString path, newPaths)
    {
        medPythonUtils::addSysPath(path);
    }
}
