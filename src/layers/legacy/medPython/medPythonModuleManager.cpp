#include "medPythonModuleManager.h"

#include <QFileInfo>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTextStream>

namespace
{

std::unique_ptr<medPythonModuleManager> _instance;

}

class medPythonModuleManagerPrivate
{
public:
    QTemporaryDir temporaryModuleFolder;
    QHash<QString, PyObject*> loadedModules;
};

medPythonModuleManager* medPythonModuleManager::instance()
{
    return _instance.get();
}

void medPythonModuleManager::initialize()
{
    _instance.reset(new medPythonModuleManager);
}

QString medPythonModuleManager::createTemporaryModuleFromString(QString moduleCode, QString moduleName)
{
    // Changing the working directory is necessary to create a QTemporaryFile in the temporary module folder
    QDir workingDirectory = QDir::current();
    QDir::setCurrent(d->temporaryModuleFolder.path());
    QScopedPointer<QFile> file;
    bool fileOpened = false;

    if (moduleName.isEmpty())
    {
        QTemporaryFile* temporaryFile = new QTemporaryFile("XXXXXX.py");
        temporaryFile->setAutoRemove(false);
        file.reset(temporaryFile);
        fileOpened = temporaryFile->open();
    }
    else
    {
        // TODO: check if file exists first
        file.reset(new QFile(moduleName + ".py"));
        fileOpened = file.data()->open(QIODevice::WriteOnly);
    }

    QFileInfo fileInfo(*file.data());
    QDir::setCurrent(workingDirectory.absolutePath());

    if (fileOpened)
    {
        QTextStream stream(file.data());
        stream << moduleCode;

        return fileInfo.baseName();
    }
    else
    {
        QString message;
        message += "Cannot open file: " + fileInfo.absoluteFilePath();
        THROW_METHOD_EXCEPTION(message.toStdString().c_str());
    }
}

void medPythonModuleManager::extractResourceModule(QString resourceLocation, QString moduleName)
{
    QFile resourceFile("://" + resourceLocation + "/" + moduleName + ".py");

    if (resourceFile.open(QIODevice::ReadOnly))
    {
        QString moduleCode;
        QTextStream stream(&resourceFile);
        moduleCode = stream.readAll();
        resourceFile.close();

        createTemporaryModuleFromString(moduleName, moduleCode);
    }
    else
    {
        THROW_METHOD_EXCEPTION("Ressource not found: " + QFileInfo(resourceFile).absoluteFilePath());
    }
}

PyObject* medPythonModuleManager::loadModule(QString moduleName, bool reload)
{
    PyObject* module;

    if (reload && d->loadedModules.contains(moduleName))
    {
        module = PyImport_ReloadModule(d->loadedModules[moduleName]);
    }
    else
    {
        module = PyImport_ImportModule(moduleName.toUtf8().constData());
    }

    if (PyErr_Occurred())
    {
        THROW_METHOD_EXCEPTION(medPythonUtils::getPythonErrorMessage());
    }

    Py_XDECREF(d->loadedModules.take(moduleName));
    d->loadedModules[moduleName] = module;
    return module;
}

medPythonModuleManager::medPythonModuleManager() :
    d(new medPythonModuleManagerPrivate())
{
    medPythonUtils::addSysPath(d->temporaryModuleFolder.path());
}

medPythonModuleManager::~medPythonModuleManager()
{
    foreach (PyObject* module, d->loadedModules.values())
    {
        Py_DECREF(module);
    }

    delete d;
}
