#include "ExternalModules.h"

#include <medSettingsManager.h>

#include "Utils.h"

namespace medPython
{

//void ModulePaths::initialize()
//{
//    updateSysPath(getPaths());
//}

void ModulePaths::setPaths(QStringList paths)
{
    QStringList oldPaths = getPaths();
    medSettingsManager::instance()->setValue("", "python_module_paths", paths);
    updateSysPath(paths, oldPaths);
}

QStringList ModulePaths::getPaths()
{
    return medSettingsManager::instance()->value("", "python_module_paths").toStringList();
}

void ModulePaths::updateSysPath(QStringList newPaths, QStringList oldPaths)
{
    foreach (QString path, oldPaths)
    {
        Utils::removeSysPath(path);
    }

    foreach (QString path, newPaths)
    {
        Utils::addSysPath(path);
    }
}

}
