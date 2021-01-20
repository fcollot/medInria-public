#pragma once

#include <QLineEdit>

#include "Export.h"

namespace medPython
{

class CommandLineBasePrivate;

class MEDPYTHON_EXPORT CommandLineBase : public QLineEdit
{
    Q_OBJECT

signals:
    void lineEntered(QString prefix, QString line);

public:
    CommandLineBase(QWidget* parent = nullptr);
    virtual ~CommandLineBase();

    void setPrefix(QString prefix);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    CommandLineBasePrivate* const d;

private slots:
    void ensureCursorAfterPrefix();

    void handleLine();

    QString currentLine();
    void setCurrentLine(QString line = "");
};

}
