#include "Resource.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "ExceptionManager.h"

namespace medPython
{

QString Resource::getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
        PythonObject message("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
        throw RuntimeError::create(MEDPYTHON_CODE_LOCATION, PythonTuple({message}));
    }

    QString sourceCode;
    QTextStream stream(&resourceFile);
    sourceCode = stream.readAll();
    resourceFile.close();
    return sourceCode;
}

QString Resource::moduleNameToResourcePath(QString name)
{
    return packageNameToResourcePath(name).append(".py");
}

QString Resource::packageNameToResourcePath(QString name)
{
    return name.prepend(":/python/").replace('.', "/");
}

}
