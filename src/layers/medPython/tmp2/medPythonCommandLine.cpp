#include "medPythonCommandLine.h"

#include <QKeyEvent>

#include "medPythonExceptionManager.h"

namespace med
{

class PythonCommandLinePrivate
{
public:
    QStringList history;
    int historyIndex;
    QString prefix;
};

PythonCommandLine::PythonCommandLine(QWidget* parent) :
    QLineEdit(parent), d(new PythonCommandLinePrivate)
{
    d->historyIndex = 0;
    d->history.append("");

    connect(this, &QLineEdit::cursorPositionChanged, this, &PythonCommandLine::ensureCursorAfterPrefix);
    connect(this, &QLineEdit::returnPressed, this, &PythonCommandLine::handleLine);
}

PythonCommandLine::~PythonCommandLine()
{
    delete d;
}

void PythonCommandLine::setPrefix(QString prefix)
{
    d->prefix = prefix;
    setCurrentLine();
}

void PythonCommandLine::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();

    if ((key == Qt::Key_Down) || (key == Qt::Key_Up))
    {
        if (key == Qt::Key_Down)
        {
            d->historyIndex = std::min(d->historyIndex + 1, d->history.length() - 1);
        }
        else
        {
            d->historyIndex = std::max(d->historyIndex - 1, 0);
        }

        setCurrentLine(d->history[d->historyIndex]);

        event->accept();
    }
    else
    {
        if ((key != Qt::Key_Backspace) || cursorPosition() > d->prefix.length())
        {
            QLineEdit::keyPressEvent(event);
        }
        else
        {
            event->accept();
        }
    }
}

void PythonCommandLine::ensureCursorAfterPrefix()
{
    setCursorPosition(std::max(cursorPosition(), d->prefix.length()));
}

void PythonCommandLine::handleLine()
{
    QString line = currentLine();
    d->history[d->history.length() - 1] = line;

    if (!line.trimmed().isEmpty())
    {
        d->history.append("");
    }

    d->historyIndex = d->history.length() - 1;
    emit lineEntered(line);
    setCurrentLine();
}

QString PythonCommandLine::currentLine()
{
    return text().mid(d->prefix.length());
}

void PythonCommandLine::setCurrentLine(QString line)
{
    setText(d->prefix + line);
}

}
