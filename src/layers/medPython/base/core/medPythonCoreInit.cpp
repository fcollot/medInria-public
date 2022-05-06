/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

// Must be included first (see the header notes for details)
#include "medPythonCoreAPI.h"

#include "medPythonCoreInit.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

#include <medExternalResources.h>

#include "medPythonCoreUtils.h"

namespace med::python
{

namespace
{

bool getAbsoluteResourcePath(QString relativePath, QString& absolutePath, QString errorMessage)
{
    absolutePath = med::getExternalResourcesDirectory(relativePath);
    bool success = !absolutePath.isEmpty();

    if (!success)
    {
        qCritical() << errorMessage;
    }

    return success;
}

bool getResourceModulesPath(QString& modulesPath)
{
    QString relativePath = "python/lib";
    QString errorMessage = QString("Cannot find the Python modules path in the application resources at %1.")
                           .arg(relativePath);
    return getAbsoluteResourcePath(relativePath, modulesPath, errorMessage);
}

bool getLibraryPath(QString& libraryPath)
{
    QDir applicationDirectory = qApp->applicationDirPath();
    QDir libraryDirectory = applicationDirectory;
    bool success = true;

#if defined(Q_OS_MACOS)
    success = libraryDirectory.cd("../Frameworks")
              && !libraryDirectory.isEmpty();

    if (!success)
    {
        libraryDirectory = applicationDirectory;
        success = libraryDirectory.cd("../../../../lib/resource_libs");
    }
#elif defined(Q_OS_LINUX)
    success = libraryDirectory.cd("../lib");
#endif

    if (success)
    {
        libraryPath = libraryDirectory.absolutePath();
    }
    else
    {
        qCritical() << QString("Cannot find the application's library path.");
    }

    return success;
}

bool getModulePaths(QStringList& modulePaths)
{
    QString libraryPath;
    QString resourceModulesPath;
    bool success = getLibraryPath(libraryPath) && getResourceModulesPath(resourceModulesPath);

    if (success)
    {
        modulePaths = QStringList{ libraryPath, resourceModulesPath };
    }

    return success;
}

bool checkStatus(PyStatus status)
{
    bool success = !PyStatus_Exception(status);

    if (!success)
    {
        qCritical() << QString("Error during initialization of Python: %1")
                       .arg(status.err_msg);
    }

    return success;
}

bool preInitialize()
{
    PyPreConfig preConfig;
    PyPreConfig_InitIsolatedConfig(&preConfig);
    preConfig.utf8_mode = 1;
    return checkStatus(Py_PreInitialize(&preConfig));
}

QStringList convertToNativePaths(QStringList paths)
{
    QStringList result;

    foreach (QString path, paths)
    {
        result << QDir::toNativeSeparators(path);
    }

    return result;
}

bool setConfigOptions(PyConfig* config, QString pythonHome, QStringList modulePaths)
{
    bool success = true;
    config->write_bytecode = 0;

    if (!pythonHome.isEmpty())
    {
        success = checkStatus(PyConfig_SetBytesString(config, &config->home, qUtf8Printable(pythonHome)));
    }

    if (success)
    {
        success = checkStatus(PyConfig_Read(config));
    }

    wchar_t** moduleSearchPaths = qStringListToWideChar(convertToNativePaths(modulePaths));

    for (int i = 0; success && (i < modulePaths.length()); i++)
    {
        success = checkStatus(PyWideStringList_Append(&config->module_search_paths, moduleSearchPaths[i]));
    }

    freeWideCharList(moduleSearchPaths, modulePaths.size());
    return success;
}

bool prepareConfig(PyConfig* config, QString pythonHome, QStringList additionalModulePaths)
{
    QStringList modulePaths;
    bool success = getModulePaths(modulePaths);

    if (success)
    {
        PyConfig_InitIsolatedConfig(config);
        success = setConfigOptions(config, pythonHome, modulePaths << additionalModulePaths);
    }

    return success;
}

bool initializeFromConfig(PyConfig* config)
{
    bool success = checkStatus(Py_InitializeFromConfig(config));

    if (success)
    {
        PyConfig_Clear(config);
    }

    return success;
}

} // namespace

bool initializeInterpreter(QString pythonHome, QStringList additionalModulePaths)
{
    Py_Initialize();
//    PyConfig config;
//    bool success = preInitialize()
//                   && prepareConfig(&config, pythonHome, additionalModulePaths)
//                   && initializeFromConfig(&config);

//    if (success)
//    {
//        qInfo() << "Embedded Python initialized: " << Py_GetVersion();
//    }
//    else
//    {
//        qCritical() << "Initialization of the embedded Python failed";
//        finalizeInterpreter();
//    }

    qInfo() << "Python initialized: " << Py_GetVersion();

    return true;
}

bool finalizeInterpreter()
{
    bool success = true;

    if (Py_IsInitialized())
    {
        success = (Py_FinalizeEx() == 0);
    }

    return success;
}

bool isInterpreterInitialized()
{
    return Py_IsInitialized();
}

} // namespace med::python
