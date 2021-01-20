#pragma once

#include <QKeySequence>
#include <QObject>

#include "PythonModule.h"

class QWidget;

namespace medPython
{

class ConsoleBase;

class MEDPYTHON_EXPORT Manager : public QObject
{
    Q_OBJECT

public:
    static const char* CONSOLE_PROPERTY_NAME;

    Manager();

    static void addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence);

    static ConsoleBase* getConsole();

public slots:
    static void toggleConsoleVisibility();
    static void teardown();

private:
    static void setupIfNeeded();

    static void setupResources();
    static void teardownResources();
    static QString getResourceFilePath();

    static void setupPythonModules();

    static void setupBindingsModule(PythonModule& package, QString name);

    static void setupResourceBasedImport(PythonModule& package);

    static PythonModule importFromResource(QString moduleName, PythonModule& package);

    static void setupOutputRedirection(PythonModule& package);

    static void setupConsole();
    static void teardownConsole();
};

}
