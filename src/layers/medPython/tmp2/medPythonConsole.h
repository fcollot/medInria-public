#pragma once

#include <QWidget>

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med
{

class PythonCommandLine;
class PythonConsolePrivate;
class PythonOutputStream;

class MEDPYTHON_EXPORT PythonConsole : public QWidget
{
    Q_OBJECT

public:
    PythonConsole(QWidget* parent = nullptr);
    virtual ~PythonConsole();

    void listenTo(PythonOutputStream* stream);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    virtual void initializeGUI();
    virtual PythonCommandLine* createCommandLine();

private:
    PythonConsolePrivate* const d;

    void updateCommandLine();

    void printLine(QString line = "");

    QString currentPrefix();

    void appendCurrentStatement(QString line);

    PythonObject runCurrentStatement();

private slots:
    void handleEnteredLine(QString line);

    void print(QString text);
};

}
