#pragma once

#include "PythonModule.h"

namespace medPython
{

class MEDPYTHON_EXPORT Resource
{
public:

    QString getResourceModuleSourceCode(QString name);

private:
    QString moduleNameToResourcePath(QString name);
    QString packageNameToResourcePath(QString name);
};

}
