/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonResourceRegistry.h"

#include <QList>

#include <medExternalResources.h>

#include "medPythonObjects.h"

namespace med::python
{

namespace
{

QList<QString> resourceLibraries;

} // namespace

void registerResourceLibrary(QString libraryName)
{
    resourceLibraries.append(libraryName);
}

void addResourceLibrariesToPath()
{
    Module sys("sys");

    foreach (QString library, resourceLibraries)
    {
        QStringList paths = {
            getExternalResourcesDirectory(library),
            getExternalResourcePath(library, "python_modules.zip")
        };

        sys.attribute("path") += list(paths);
    }
}

} // namespace med::python
