#include "ResourceMModuleFactory.h"

#include <QResource>
#include <QFileInfo>
#include <QTextStream>

#include "Convert.h"
#include "Exception.h"

namespace medPython
{



QString Resource::getResourceModuleSourceCode(QString name)
{
    QFile resourceFile(moduleNameToResourcePath(name));

    if (!resourceFile.open(QIODevice::ReadOnly))
    {
        THROW_PYTHON_RELATED_EXCEPTION("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
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
