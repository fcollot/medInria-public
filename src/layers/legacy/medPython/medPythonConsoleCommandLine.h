#pragma once

#include <QLineEdit>

#include "medPythonExport.h"

class medPythonConsoleCommandLinePrivate;

class MEDPYTHON_EXPORT medPythonConsoleCommandLine : public QLineEdit
{
    Q_OBJECT

public:
    medPythonConsoleCommandLine(QWidget* parent = nullptr);
    ~medPythonConsoleCommandLine();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    medPythonConsoleCommandLinePrivate* const d;
};
