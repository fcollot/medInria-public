/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonTools.h"
#include <medExternalResources.h>
#include <medPython.h>
#include <QDebug>
namespace med::python
{

void registerToolsPaths()
{
    Module sys("sys");

    QStringList paths = {
        getExternalResourcesDirectory(TARGET_NAME),
        getExternalResourcePath(QString(TARGET_NAME) + ".zip", TARGET_NAME)
    };

    sys.attribute("path") += list(paths);
}

void initializeTools()
{
    coreFunction(PyRun_SimpleString, "from medInria_medPythonTools import *");

    Module medPython(PYTHON_PACKAGE_NAME);
    Object console = medPython.callMethod("createConsole")
                     .kwarg("title", QString(CONSOLE_TITLE))
                     .kwarg("size", list<int>({CONSOLE_WIDTH, CONSOLE_HEIGHT}));
    console.callMethod("setShortcut", QString(CONSOLE_SHORTCUT));
    console.callMethod("run");
}

} // namespace med::python
