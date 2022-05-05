/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonInit.h"

#include <QApplication>

#include "medPython.h"
#include "medPythonCoreAPI.h"
#include "medPythonCoreInit.h"

namespace med::python
{

bool initializeCore()
{
    bool success = true;

    if (!isRunning())
    {
        success = initializeInterpreter(getUserPythonPaths());

        if (success)
        {
            initializeExceptions();
            QApplication::connect(qApp, &QApplication::aboutToQuit, &finalize);
        }
    }

    return success;
}

bool initializeTools()
{
    bool success = false;

    if (isRunning())
    {
        try
        {
            import("medPythonTools").callMethod("initialize");
            success = true;
        }
        catch (Exception& e)
        {
            qCritical() << QString("Error during initialization of medPythonTools: %1").arg(e.what());
        }
    }

    return success;
}

bool loadPlugins()
{
    bool success = false;

    if (isRunning())
    {
        try
        {
            import("medPythonTools").callMethod("loadPlugins");

            // Allow user to query info on successful and failed plugins.
            QString command = "from medPythonTools import pluginsInfo";
            coreFunction(PyRun_SimpleString, qUtf8Printable(command));

            success = true;
        }
        catch (Exception& e)
        {
            qCritical() << QString("Error while loading the Python plugins: %1").arg(e.what());
        }
    }

    return success;
}

bool runConsole()
{
    bool success = false;

    if (isRunning())
    {
        try
        {
            import("medPythonTools").callMethod("initializeConsole");
            success = true;
        }
        catch (Exception& e)
        {
            qCritical() << QString("Error while starting the Python console: %1").arg(e.what());
        }
    }

    return success;
}

bool finalize()
{
    bool success = true;

    if (isRunning())
    {
        finalizeExceptions();
        success = finalizeInterpreter();
    }

    return success;
}

bool isRunning()
{
    return isInterpreterInitialized();
}

} // namespace med::python
