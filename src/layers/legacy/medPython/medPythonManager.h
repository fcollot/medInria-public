#pragma once

#include <QKeySequence>
#include <QObject>

#include "medPythonExport.h"

class QWidget;

class medPythonConsole;
class medPythonManagerPrivate;
class medPythonModuleHandle;

class MEDPYTHON_EXPORT medPythonManager : public QObject
{
    Q_OBJECT
public:
    static const char* CONSOLE_PROPERTY_NAME;

    medPythonManager();

    static void addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence);

    static medPythonConsole* getConsole();

public slots:
    static void toggleConsoleVisibility();
    static void teardown();

private:
    static void setupIfNeeded();

    static void setupResources();
    static void teardownResources();
    static QString getResourceFilePath();

    static void setupPythonModules();

    static void setupBindingsModule(medPythonModuleHandle& package);

    static void setupResourceImportModule(medPythonModuleHandle& package);

    static void setupOutputRedirection(medPythonModuleHandle& package);

    static void setupConsole();
    static void teardownConsole();
};
