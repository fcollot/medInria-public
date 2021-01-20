#pragma once

#include <QWidget>

#include "Export.h"

namespace medPython
{

class CommandLineBase;
class ConsoleBasePrivate;
class PythonOutputStream;

class MEDPYTHON_EXPORT ConsoleBase : public QWidget
{
    Q_OBJECT

public:
    ConsoleBase(QWidget* parent = nullptr);
    virtual ~ConsoleBase();

    void listenTo(PythonOutputStream* stream);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    virtual void initializeGUI();
    virtual CommandLineBase createCommandLine() = 0;

private:
    ConsoleBasePrivate* const d;

    void setDefaultCommandLinePrefix();

private slots:
    void handleEnteredLine(QString prefix, QString line);

    void print(QString text);
};

}
