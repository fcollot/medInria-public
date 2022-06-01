/*==============================================================================

 medInria

 Copyright (c) INRIA 2022. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonCoreAPI.h"

#include "medPythonManager.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QKeySequence>

#include "medPython.h"

namespace med::python
{

struct PythonManagerPrivate
{
    static QStringList buildPaths;
    static QStringList packagePaths;

    bool pythonIsUsable = false;
    Object tools;
    Object console;
};

QStringList PythonManagerPrivate::buildPaths =
{
#if defined(Q_OS_MACOS)
    "<application_dir>/../../../../resources/<python_modules_dir>",
    "<application_dir>/../../../../<resource_libs_dir>"
#else
    "<application_dir>/../resources/<python_modules_dir>",
    "<application_dir>/../<resource_libs_dir>"
#endif
};

QStringList PythonManagerPrivate::packagePaths =
{
#if defined(Q_OS_MACOS)
    "<application_dir>/Resources/<python_modules_dir>",
    "<application_dir>/Frameworks"
#elif defined(Q_OS_LINUX)
    "<application_dir>/resources/<python_modules_dir>",
    "<application_dir>/lib"
#else
    "<application_dir>/resources/<python_modules_dir>",
    "<application_dir>"
#endif
};

PythonManager::PythonManager() :
    d(new PythonManagerPrivate)
{
    initialize();
}

PythonManager::~PythonManager()
{
    finalize();
}

bool PythonManager::pythonIsUsable()
{
    return d->pythonIsUsable;
}

void PythonManager::loadPlugins()
{
    if (d->pythonIsUsable)
    {
        try
        {
            d->tools.callMethod("loadPlugins");
        }
        catch (Exception& e)
        {
            qCritical() << QString("Error while loading the Python plugins: %1").arg(e.what());
        }
    }
}

void PythonManager::startConsole()
{
    if (d->pythonIsUsable)
    {
        try
        {
            Object size = tuple<int>({consoleWidth, consoleHeight});
            d->console = d->tools.attribute("Console")(size);
            QObject* mainWindow = qApp->property("MainWindow").value<QObject*>();
            QObject::connect(mainWindow, &QObject::destroyed, [&]() { d->console.callMethod("deleteLater"); d->console = nullptr; });
            d->console.callMethod("run");
            d->console.callMethod("setShortcut", Object(consoleShortCut));
            qInfo() << QString("The Python console can be accessed with %1.")
                       .arg(QKeySequence(consoleShortCut).toString(QKeySequence::NativeText));
        }
        catch (Exception& e)
        {
            qCritical() << QString("Error while starting the Python console: %1").arg(e.what());
        }
    }
}

void PythonManager::initialize()
{
    try
    {
        initializeInterpreter();
        initializeAPI();
        initializePaths();
        initializeTools();
        d->pythonIsUsable = true;
    }
    catch (Exception& e)
    {
        qCritical() << QString("Initialization of Python failed: %1").arg(e.what());
    }
}

void PythonManager::initializeInterpreter()
{
    Py_Initialize();

    if (Py_IsInitialized())
    {
        qInfo() << "Python interpreter initialized: " << Py_GetVersion();
    }
    else
    {
        throw RuntimeError("Initialization of Python interpreter failed.");
    }
}

void PythonManager::initializeAPI()
{
    initializeExceptions();
}

void PythonManager::initializePaths()
{
    QStringList paths = getInternalPaths(d->packagePaths);

    if (!testInternalPaths(paths))
    {
        paths = getInternalPaths(d->buildPaths);

        if (!testInternalPaths(paths))
        {
            throw RuntimeError("Cannot find the interal module paths.");
        }
    }

    foreach (QString path, paths)
    {
        addPythonPath(path);
    }
}

void PythonManager::initializeTools()
{
    d->tools = import("medPythonTools");
    d->tools.callMethod("initialize");

    // Allow user to query info on successful and failed plugins.
    QString command = "from medPythonTools import pluginsInfo";
    coreFunction(PyRun_SimpleString, qUtf8Printable(command));
}

void PythonManager::finalize()
{
    finalizeInterpreter();
    d->pythonIsUsable = false;
}

void PythonManager::finalizeInterpreter()
{
    if (Py_IsInitialized())
    {
        Py_FinalizeEx();
        qInfo() << "Python interpreter terminated.";
    }
}

QStringList PythonManager::getInternalPaths(QStringList pathDefinitions)
{
    QStringList result;
    QString applicationDir = qApp->applicationDirPath();

    foreach (QString path, pathDefinitions)
    {
        path.replace("<application_dir>", applicationDir);
        path.replace("<python_modules_dir>", PYTHON_MODULE_RESOURCE_DIR);
        path.replace("<resource_libs_dir>", RESOURCE_LIB_DIR);
        result << QDir(path).absolutePath();
    }

    return result;
}

bool PythonManager::testInternalPaths(QStringList paths)
{
    bool success = true;

    foreach (QDir path, paths)
    {
        if (!path.exists() || path.isEmpty())
        {
            success = false;
            break;
        }
    }

    return success;
}

} // namespace med::python
