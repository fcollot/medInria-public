/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPipelinePlugin.h"

#include <medExternalResources.h>
#include <medPython.h>

namespace med::pipeline
{

Plugin::Plugin(QObject *parent) : medPluginLegacy(parent)
{
    python::registerModulePath(getExternalResourcePath(QString(TARGET_NAME) + ".zip", TARGET_NAME));
    python::import(PYTHON_PACKAGE_NAME);
}

bool Plugin::initialize()
{
    return true;
}

QString Plugin::name() const
{
    return TARGET_NAME;
}

QString Plugin::description() const
{
    return tr("Pipeline workspace");
}

QString Plugin::version() const
{
    return TARGET_VERSION;
}

QStringList Plugin::types() const
{
    return QStringList();
}

} // namespace med::pipeline
