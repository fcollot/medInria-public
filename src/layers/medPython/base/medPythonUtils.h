#pragma once
/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include <QString>

#include "medPythonObjects.h"
#include "medPythonExport.h"

namespace med::python
{

MEDPYTHON_EXPORT void registerModulePath(QString path);

MEDPYTHON_EXPORT void loadPlugins();

MEDPYTHON_EXPORT Object runSourceCode(QString sourceCode);

} // namespace med::python
