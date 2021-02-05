#include "medPythonManager.h"

#include <QApplication>
#include <QShortcut>

#include <medSettingsWidgetFactory.h>

#include "medExternalPythonModules.h"
#include "medPythonConsole.h"
#include "medPythonExceptionManager.h"
#include "medPythonResource.h"
#include "medPythonSettingsWidget.h"
#include "medPythonSequenceBackend.h"

namespace med
{

const char* PythonManager::CONSOLE_PROPERTY_NAME = "PythonConsole";

PythonManager::PythonManager()
{
    setupIfNeeded();
}

void PythonManager::addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence)
{
    QShortcut* consoleShortcut = new QShortcut(keySequence, targetWidget);
    consoleShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(consoleShortcut, &QShortcut::activated, &PythonManager::toggleConsoleVisibility);
    qInfo() << "The Python console can be accessed with " + keySequence.toString();
}

PythonConsole* PythonManager::getConsole()
{
    return qApp->property(CONSOLE_PROPERTY_NAME).value<PythonConsole*>();
}

void PythonManager::toggleConsoleVisibility()
{
    PythonConsole* console = getConsole();
    console->setVisible(!console->isVisible());
}

void PythonManager::teardown()
{
    if (Py_IsInitialized())
    {
        Py_Finalize();

        teardownConsole();
        teardownResources();
    }
}

void PythonManager::setupIfNeeded()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();

        PythonErrorManager::initialize();
        ExternalPythonModules::initialize();

        setupConsole();
        setupResources();
        setupPythonModules();

        medSettingsWidgetFactory::instance()->registerSettingsWidget<PythonSettingsWidget>();

        QObject* mainWindow = qApp->property("MainWindow").value<QObject*>();
        connect(mainWindow, &QObject::destroyed, &PythonManager::teardown);
    }
}

void PythonManager::setupResources()
{
    QResource::registerResource(getResourceFilePath());
}

void PythonManager::teardownResources()
{
    QResource::unregisterResource(getResourceFilePath());
}

QString PythonManager::getResourceFilePath()
{
    return qEnvironmentVariable("MEDINRIA_RESOURCES_DIR") + "/" + RESOURCE_FILE_NAME;
}

void PythonManager::setupPythonModules()
{
    PythonModule package = PythonModule().createPackage(PYTHON_PACKAGE_NAME);
    setupBindingsModule(package, QT_BINDINGS_MODULE);
    setupBindingsModule(package, LOCAL_BINDINGS_MODULE);
    setupBindingsModule(package, GUI_BINDINGS_MODULE);
    //setupOutputRedirection(package);

    //Utils::addSysPath(":/python");
    //Utils::runPythonString("import \nprint(str(.__dict__))");
    //Utils::runPythonString("import sys\nprint(str(sys.path_hooks))");
}

void PythonManager::setupBindingsModule(PythonModule& package, QString name)
{
    PythonModule module = package.createSubModule(name);
    module.runCode(PythonResource::getResourceModuleSourceCode(module.getName()));
}

void PythonManager::setupConsole()
{
    PythonModule medInriaPackage;
    PythonType consoleType = medInriaPackage.getAttribute("PythonConsole");
    PythonConsole* console = consoleType().convert<PythonConsole>();
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant::fromValue(console));
}

void PythonManager::teardownConsole()
{
    delete getConsole();
    qApp->setProperty(CONSOLE_PROPERTY_NAME, QVariant());
}

}
