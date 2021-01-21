#pragma once

#include "PythonModule.h"

namespace medPython
{

class MEDPYTHON_EXPORT Resource
{
public:
    static QString getResourceModuleSourceCode(QString name);

    static QString moduleNameToResourcePath(QString name);

    static QString packageNameToResourcePath(QString name);
};

}
