#include "Manager.h"

#include <QApplication>
#include <QShortcut>

#include <medSettingsWidgetFactory.h>

#include "Console.h"
#include "Resource.h"
#include "SettingsWidget.h"
#include "PythonType.h"

namespace medPython
{

const char* Manager::CONSOLE_PROPERTY_NAME = "PythonConsole";

Manager::Manager()
{
    setupIfNeeded();
}

void Manager::addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence)
{
    QShortcut* consoleShortcut = new QShortcut(keySequence, targetWidget);
    consoleShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(consoleShortcut, &QShortcut::activated, &Manager::toggleConsoleVisibility);
    qInfo() << "The Python console can be accessed with " + keySequence.toString();
}

ConsoleBase* Manager::getConsole()
{
    return qApp->property(CONSOLE_PROPERTY_NAME).value<ConsoleBase*>();
}

void Manager::toggleConsoleVisibility()
{
    ConsoleBase* console = getConsole();
    console->setVisible(!console->isVisible());
}

void Manager::teardown()
{
    if (Py_IsInitialized())
    {
        Py_Finalize();

        teardownConsole();
        teardownResources();
    }
}

void Manager::setupIfNeeded()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();

        setupConsole();
        setupResources();
        setupPythonModules();

        medSettingsWidgetFactory::instance()->registerSettingsWidget<SettingsWidget>();

        QObject* mainWindow = qApp->property("MainWindow").value<QObject*>();
        connect(mainWindow, &QObject::destroyed, &Manager::teardown);
    }
}

void Manager::setupResources()
{
    QResource::registerResource(getResourceFilePath());
}

void Manager::teardownResources()
{
    QResource::unregisterResource(getResourceFilePath());
}

QString Manager::getResourceFilePath()
{
    return qEnvironmentVariable("MEDINRIA_RESOURCES_DIR") + "/" + RESOURCE_FILE_NAME;
}

void Manager::setupPythonModules()
{
    PythonModule package = Resource().createPackage(PYTHON_PACKAGE_NAME);
    setupBindingsModule(package, _QT_BINDINGS);
    setupBindingsModule(package, _BINDINGS);
    setupBindingsModule(package, medInria_GUI_BINDINGS);
    setupResourceBasedImport(package);
    //setupOutputRedirection(package);

    Utils::addSysPath(":/python");
    //Utils::runPythonString("import \nprint(str(.__dict__))");
    //Utils::runPythonString("import sys\nprint(str(sys.path_hooks))");
}
#include <QDebug>
void Manager::setupBindingsModule(PythonModule& package, QString name)
{
    PythonModule module = package.createSubModule(name);
    module.runCode(Resource().getResourceModuleSourceCode(module.getName()));
    //package.importObjectsFrom(module, {"*"});
    //PyObject_SetAttrString(package.getModuleObject(), TO_CSTRING(name), module.getModuleObject());
}

void Manager::setupResourceBasedImport(PythonModule& package)
{
    PythonModule resourceImportModule = importFromResource("resource_import", package);


//    ModuleHandle bindingsModule = ModuleHandle::createModule(moduleName);
    //    bindingsModule.runCode(ModuleHandle::getResourceModuleSourceCode(moduleName));
}

void Manager::importFromResource(QString moduleName, PythonModule& package)
{
    PythonModule module = package.createSubModule(moduleName);
    module.runCode(Resource().getResourceModuleSourceCode(module.getName()));
    return module;
}

void Manager::setupOutputRedirection(PythonModule& package)
{
    PythonModule module = package.createSubModule("console");
    module.runCode(Resource().getResourceModuleSourceCode(module.getName()));
}

void Manager::setupConsole()
{
    PythonModule medInriaPackage;
    PythonType consoleType = medInriaPackage.getAttribute("PythonConsole");
    ConsoleBase* console = consoleType.newCastedInstance<ConsoleBase>();
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant::fromValue(console));
}

void Manager::teardownConsole()
{
    delete getConsole();
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant());
}

}
