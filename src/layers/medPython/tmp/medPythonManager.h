#pragma once

#include <QKeySequence>
#include <QObject>

class QWidget;

namespace med
{

class PythonConsole;

class MEDPYTHON_EXPORT PythonManager : public QObject
{
    Q_OBJECT

public:
    static const char* CONSOLE_PROPERTY_NAME;

    PythonManager();

    static void addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence);

    static PythonConsole* getConsole();

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

    static void setupConsole();
    static void teardownConsole();
};

}
