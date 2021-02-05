#pragma once

#include <QString>

#include "medPythonExport.h"

namespace med
{
namespace python
{

QString getResourceModuleSourceCode(QString name);

QString moduleNameToResourcePath(QString name);

QString packageNameToResourcePath(QString name);


}
}
