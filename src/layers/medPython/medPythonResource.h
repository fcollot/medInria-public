#pragma once

#include <QString>

#include "medPythonExport.h"

namespace med
{

class MEDPYTHON_EXPORT PythonResource
{
public:
    static QString getResourceModuleSourceCode(QString name);

    static QString moduleNameToResourcePath(QString name);

    static QString packageNameToResourcePath(QString name);

private:
    PythonResource() = default;
    ~PythonResource() = default;
};

}
