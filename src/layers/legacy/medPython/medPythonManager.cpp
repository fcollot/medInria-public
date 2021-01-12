#include "medPythonManager.h"

#include <QApplication>
#include <QShortcut>

#include <medSettingsWidgetFactory.h>

#include "medPythonConsole.h"
#include "medPythonModuleFactory.h"
#include "medPythonSettingsWidget.h"
#include "medPythonUtils.h"

const char* medPythonManager::CONSOLE_PROPERTY_NAME = "PythonConsole";

medPythonManager::medPythonManager()
{
    setupIfNeeded();
}

void medPythonManager::addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence)
{
    QShortcut* consoleShortcut = new QShortcut(keySequence, targetWidget);
    consoleShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(consoleShortcut, &QShortcut::activated, &medPythonManager::toggleConsoleVisibility);
    qInfo() << "The Python console can be accessed with " + keySequence.toString();
}

medPythonConsole* medPythonManager::getConsole()
{
    return qApp->property(CONSOLE_PROPERTY_NAME).value<medPythonConsole*>();
}

void medPythonManager::toggleConsoleVisibility()
{
    medPythonConsole* console = getConsole();
    console->setVisible(!console->isVisible());
}

void medPythonManager::teardown()
{
    if (Py_IsInitialized())
    {
        Py_Finalize();

        teardownConsole();
        teardownResources();
    }
}

void medPythonManager::setupIfNeeded()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();

        setupConsole();
        setupResources();
        setupPythonModules();

        medSettingsWidgetFactory::instance()->registerSettingsWidget<medPythonSettingsWidget>();

        QObject* mainWindow = qApp->property("MainWindow").value<QObject*>();
        connect(mainWindow, &QObject::destroyed, &medPythonManager::teardown);
    }
}

void medPythonManager::setupResources()
{
    QResource::registerResource(getResourceFilePath(), "/python");
}

void medPythonManager::teardownResources()
{
    QResource::unregisterResource(getResourceFilePath(), "/python");
}

QString medPythonManager::getResourceFilePath()
{
    return qEnvironmentVariable("MEDINRIA_RESOURCES_DIR") + "/medPython.rcc";
}

void medPythonManager::setupPythonModules()
{
    medPythonModuleHandle package = medPythonModuleFactory().createPackage(TARGET_NAME);
    setupBindingsModule(package);
    setupResourceImportModule(package);
    setupOutputRedirection(package);

    medPythonUtils::addSysPath(":/python");
    //medPythonUtils::runPythonString("import medPython\nprint(str(medPython.__dict__))");
    //medPythonUtils::runPythonString("import sys\nprint(str(sys.path_hooks))");
}

void medPythonManager::setupBindingsModule(medPythonModuleHandle& package)
{
    medPythonModuleHandle module = package.createModule(TARGET_BINDINGS);
    module.runCode(medPythonModuleFactory().getResourceModuleSourceCode(module.getName()));
    package.importObjectsFrom(module, {"*"});
}

void medPythonManager::setupResourceImportModule(medPythonModuleHandle& package)
{
    QString moduleName = package.getName() + ".resource_import";
//    medPythonModuleHandle bindingsModule = medPythonModuleHandle::createModule(moduleName);
    //    bindingsModule.runCode(medPythonModuleHandle::getResourceModuleSourceCode(moduleName));
}

void medPythonManager::setupOutputRedirection(medPythonModuleHandle& package)
{
    medPythonModuleHandle module = package.createModule("console");
    module.runCode(medPythonModuleFactory().getResourceModuleSourceCode(module.getName()));
}

void medPythonManager::setupConsole()
{
    medPythonConsole* console = new medPythonConsole;
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant::fromValue(console));
}

void medPythonManager::teardownConsole()
{
    delete getConsole();
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant());
}
