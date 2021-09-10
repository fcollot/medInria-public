/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonCoreAPI.h"

#include "medPythonPlugins.h"

#include <QDebug>

#include "medPythonCoreFunction.h"
#include "medPythonObjects.h"

namespace med::python
{

void loadPlugins()
{
    Module pkgutil("pkgutil");
    Object moduleIterator = pkgutil.callMethod("iter_modules");
    Object moduleInfo = coreFunction(PyIter_Next, *moduleIterator);

    while (moduleInfo)
    {
        QString moduleName = moduleInfo[1].convert<QString>();

        if (moduleName.startsWith("medInria_"))
        {
            Module plugin(moduleName);
        }

        moduleInfo = coreFunction(PyIter_Next, *moduleIterator);
    }
}

} // namespace med::python
