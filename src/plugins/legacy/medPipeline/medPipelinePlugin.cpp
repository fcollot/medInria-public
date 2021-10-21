/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPipelinePlugin.h"

#include "medAbstractDataFactory.h"

#include <medExternalResources.h>
#include <medPython.h>

namespace med::pipeline
{

namespace
{

QString staticIdentifier(QString className)
{
    python::Object identifier = python::import(PYTHON_PACKAGE_NAME).attribute(className).callMethod("staticIdentifier");
    return identifier.convert<QString>();
}

template <class TYPE>
TYPE* createPythonInstance(QString className)
{
    python::Object instance = python::import(PYTHON_PACKAGE_NAME).attribute(className)();
    return instance.cast<TYPE>();
}

} // namespace

Plugin::Plugin(QObject *parent) : medPluginLegacy(parent)
{
    python::registerModulePath(getExternalResourcePath(QString(TARGET_NAME) + ".zip", TARGET_NAME));
    python::Module pipelineModule = python::import(PYTHON_PACKAGE_NAME);

    medAbstractDataFactory::instance()->registerDataType(staticIdentifier("JSONData"),
                                                         []() { return createPythonInstance<dtkAbstractData>("JSONData"); });
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
