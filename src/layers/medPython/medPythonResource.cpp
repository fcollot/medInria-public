#include "medPythonResource.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "medPythonExceptions.h"
#include "medPythonObject.h"

namespace med
{

QString PythonResource::getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
        PythonObject message = PythonObject::create("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
        throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
    }

    QString sourceCode;
    QTextStream stream(&resourceFile);
    sourceCode = stream.readAll();
    resourceFile.close();
    return sourceCode;
}

QString PythonResource::moduleNameToResourcePath(QString name)
{
    return packageNameToResourcePath(name).append(".py");
}

QString PythonResource::packageNameToResourcePath(QString name)
{
    return name.prepend(":/python/").replace('.', "/");
}

}
