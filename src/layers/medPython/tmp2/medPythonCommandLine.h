#pragma once

#include <QLineEdit>

#include "medPythonExport.h"

namespace med
{

class PythonCommandLinePrivate;

class MEDPYTHON_EXPORT PythonCommandLine : public QLineEdit
{
    Q_OBJECT

signals:
    void lineEntered(QString line);

public:
    PythonCommandLine(QWidget* parent = nullptr);
    virtual ~PythonCommandLine();

    void setPrefix(QString prefix);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    PythonCommandLinePrivate* const d;

private slots:
    void ensureCursorAfterPrefix();

    void handleLine();

    QString currentLine();
    void setCurrentLine(QString line = "");
};

}
