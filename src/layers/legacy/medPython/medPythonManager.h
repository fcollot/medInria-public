#pragma once

#include <QKeySequence>
#include <QObject>

#include "medPythonExport.h"

class QWidget;

class medPythonManagerPrivate;
class medPythonModuleHandle;

class MEDPYTHON_EXPORT medPythonManager : public QObject
{
    Q_OBJECT

public:
    medPythonManager(QObject* parent = nullptr);
    virtual ~medPythonManager();

    void addConsoleShortcut(QWidget* targetWidget, QKeySequence keySequence);



private:
    medPythonManagerPrivate* const d;

    void setupPythonModules();
    void setupBindingsModule(medPythonModuleHandle& package);
    void setupResourceImportModule(medPythonModuleHandle& package);
    void setupConsole();

private slots:
    void toggleConsoleVisibility();
};
