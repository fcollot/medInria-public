#include "medPythonManager.h"

#include <QShortcut>

#include <medSettingsWidgetFactory.h>

#include "medPythonConsoleWidget.h"
#include "medPythonError.h"
#include "medPythonModuleHandle.h"
#include "medPythonModulePaths.h"
#include "medPythonSettingsWidget.h"
#include "medPythonUtils.h"

class medPythonManagerPrivate
{
public:
    medPythonConsoleWidget console;
    QShortcut consoleShortcut;
};

medPythonManager::medPythonManager(QObject* parent) :
    QObject(parent), d(new medPythonManagerPrivate)
{


    if (!Py_IsInitialized())
    {
        Py_Initialize();

        medPythonErrorChecker::initialize();
        medPythonModulePaths::initialize();
        medSettingsWidgetFactory::instance()->registerSettingsWidget<medPythonSettingsWidget>();
        setupPythonModules();
        setupConsole();
    }
}

medPythonManager::~medPythonManager()
{
    d->instanceCount--;

    if ((d->instanceCount == 0) && Py_IsInitialized())
    {
        Py_Finalize();
    }

    delete d;
}

void medPythonManager::addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence)
{
    QShortcut* consoleShortcut = new QShortcut(keySequence, targetWidget);
    consoleShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(consoleShortcut, &QShortcut::activated, this, &medPythonManager::toggleConsoleVisibility);
    qInfo() << "The Python console can be accessed with " + QKeySequence.toString();
}

void medPythonManager::setupPythonModules()
{
    QString resourcesDir = qEnvironmentVariable("MEDINRIA_RESOURCES_DIR");
    QResource::registerResource(resourcesDir + "/medPython.rcc");

    medPythonModuleHandle package = medPythonModuleHandle::createPackage(TARGET_NAME);
    setupBindingsModule(package);
    setupResourceImportModule(package);

    medPythonUtils::addSysPath(":/python");
    //medPythonUtils::runPythonString("import medPython\nprint(str(medPython.__dict__))");
    //medPythonUtils::runPythonString("import sys\nprint(str(sys.path_hooks))");
}

void medPythonManager::setupBindingsModule(medPythonModuleHandle& package)
{
    QString moduleName = package.getName() + "." + TARGET_BINDINGS;
    medPythonModuleHandle bindingsModule = medPythonModuleHandle::createModule(moduleName);
    bindingsModule.runString(medPythonModuleHandle::getResourceModuleSourceCode(moduleName));
    package.importObjectsFrom(bindingsModule);
}

void medPythonManager::setupResourceImportModule(medPythonModuleHandle& package)
{
    QString moduleName = package.getName() + ".resource_import";
    medPythonModuleHandle bindingsModule = medPythonModuleHandle::createModule(moduleName);
    bindingsModule.runString(medPythonModuleHandle::getResourceModuleSourceCode(moduleName));
}

void medPythonManager::setupConsole()
{
    d->console = new medPythonConsoleWidget();
    medPythonUtils::runPythonString("print(sys.version)");
}

void medPythonManager::toggleConsoleVisibility()
{
    d->console->setVisible(!d->console->isVisible());
}
