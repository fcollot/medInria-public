#include "medPythonResource.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "medPythonExceptions.h"
#include "medPythonObject.h"

namespace med
{
namespace python
{

QString getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
         throw RuntimeError(MEDPYTHON_CODE_LOCATION, QString("Ressource not found: %1").arg(QFileInfo(resourceFile).absoluteFilePath()));
    }

    QString sourceCode;
    QTextStream stream(&resourceFile);
    sourceCode = stream.readAll();
    resourceFile.close();
    return sourceCode;
}

QString moduleNameToResourcePath(QString name)
{
    return packageNameToResourcePath(name).append(".py");
}

QString packageNameToResourcePath(QString name)
{
    return name.prepend(":/python/").replace('.', "/");
}

}
}
