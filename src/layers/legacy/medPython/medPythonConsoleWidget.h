#pragma once

#include <QWidget>

#include "medPythonExport.h"

class medPythonConsoleWidgetPrivate;

class MEDPYTHON_EXPORT medPythonConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    medPythonConsoleWidget();
    ~medPythonConsoleWidget();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    medPythonConsoleWidgetPrivate* const d;

    void initializeGUI();

private slots:
    void handleStatement();

    void print(QString text);
};
