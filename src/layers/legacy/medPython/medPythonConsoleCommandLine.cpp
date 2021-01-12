#include "medPythonConsoleCommandLine.h"

#include <QKeyEvent>

#include "medPythonError.h"

class medPythonConsoleCommandLinePrivate
{
public:
    QStringList history;
    int historyIndex;
};

medPythonConsoleCommandLine::medPythonConsoleCommandLine(QWidget* parent) :
    QLineEdit(parent), d(new medPythonConsoleCommandLinePrivate)
{
    d->historyIndex = 0;
    d->history.append("");
}

medPythonConsoleCommandLine::~medPythonConsoleCommandLine()
{
    delete d;
}

void medPythonConsoleCommandLine::keyPressEvent(QKeyEvent* event)
{
    d->history[d->historyIndex] = text();
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

        setText(d->history[d->historyIndex]);

        event->accept();
    }
    else
    {
        if (key == Qt::Key_Return)
        {
            d->history.append(text());
            d->historyIndex++;
        }

        QLineEdit::keyPressEvent(event);
    }
}
