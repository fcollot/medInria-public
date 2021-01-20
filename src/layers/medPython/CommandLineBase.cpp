#include "CommandLineBase.h"

#include <QKeyEvent>

#include "Exception.h"

namespace medPython
{

class CommandLineBasePrivate
{
public:
    QStringList history;
    int historyIndex;
    QString prefix;
};

CommandLineBase::CommandLineBase(QWidget* parent) :
    QLineEdit(parent), d(new CommandLineBasePrivate)
{
    d->historyIndex = 0;
    d->history.append("");

    connect(this, &QLineEdit::cursorPositionChanged, this, &CommandLineBase::ensureCursorAfterPrefix);
    connect(this, &QLineEdit::returnPressed, this, &CommandLineBase::handleLine);
}

CommandLineBase::~CommandLineBase()
{
    delete d;
}

void CommandLineBase::setPrefix(QString prefix)
{
    d->prefix = prefix;
    setCurrentLine();
}

void CommandLineBase::keyPressEvent(QKeyEvent* event)
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

void CommandLineBase::ensureCursorAfterPrefix()
{
    setCursorPosition(std::max(cursorPosition(), d->prefix.length()));
}

void CommandLineBase::handleLine()
{
    QString line = currentLine();
    d->history[d->history.length() - 1] = line;

    if (!line.trimmed().isEmpty())
    {
        d->history.append("");
    }

    d->historyIndex = d->history.length() - 1;
    emit lineEntered(d->prefix, line);
    setCurrentLine();
}

QString CommandLineBase::currentLine()
{
    return text().mid(d->prefix.length());
}

void CommandLineBase::setCurrentLine(QString line)
{
    setText(d->prefix + line);
}

}
