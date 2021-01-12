#pragma once

#include <QWidget>

#include "medPythonExport.h"

class medPythonConsolePrivate;
class medPythonOutputStream;

class MEDPYTHON_EXPORT medPythonConsole : public QWidget
{
    Q_OBJECT

public:
    medPythonConsole(QWidget* parent = nullptr);
    ~medPythonConsole();

    void listenTo(medPythonOutputStream* stream);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    medPythonConsolePrivate* const d;

    void initializeGUI();

private slots:
    void handleStatement();

    void printCommand(QString text);
    void print(QString text);
};
