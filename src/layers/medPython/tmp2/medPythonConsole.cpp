#include "medPythonConsole.h"

#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>

#include "medPythonCommandLine.h"
#include "medPythonExceptionManager.h"
#include "medPythonModule.h"
#include "medPythonOutputStream.h"

namespace med
{

class PythonConsolePrivate
{
public:
    static const char* promptPrefix;
    static const char* continuationPrefix;

    QTextEdit* outputText;
    PythonCommandLine* commandLine;
    QString currentStatement;
    bool statementFinished = false;
    QStringList statementHistory;
};

const char* PythonConsolePrivate::promptPrefix = ">>> ";
const char* PythonConsolePrivate::continuationPrefix = "... ";

PythonConsole::PythonConsole(QWidget* parent) :
    QWidget(parent), d(new PythonConsolePrivate)
{
    initializeGUI();
}

PythonConsole::~PythonConsole()
{
    delete d;
}

void PythonConsole::listenTo(PythonOutputStream* stream)
{
    connect(stream, &PythonOutputStream::recieved, this, &PythonConsole::print);
}

void PythonConsole::showEvent(QShowEvent* event)
{
    d->commandLine->setFocus();
    event->accept();
}

void PythonConsole::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void PythonConsole::initializeGUI()
{
    setWindowTitle("Python console");

    setLayout(new QVBoxLayout);
    resize(800, 600);

    d->outputText = new QTextEdit;
    layout()->addWidget(d->outputText);
    d->outputText->setReadOnly(true);
    d->outputText->setUndoRedoEnabled(false);

    d->commandLine = createCommandLine();
    layout()->addWidget(d->commandLine);
    connect(d->commandLine, &PythonCommandLine::lineEntered, this, &PythonConsole::handleEnteredLine);

    QTextDocument *textDocument = d->outputText->document();
    QFont font = textDocument->defaultFont();
    font.setFamily("Courier");
    textDocument->setDefaultFont(font);
    d->commandLine->setFont(font);

    int numVisibleLines = height()/fontMetrics().height();

    for (int i = 0; i < numVisibleLines; i++)
    {
        print("\n");
    }

    updateCommandLine();
}

PythonCommandLine* PythonConsole::createCommandLine()
{
    return new PythonCommandLine;
}

void PythonConsole::updateCommandLine()
{
    d->commandLine->setPrefix(currentPrefix());
}

void PythonConsole::printLine(QString line)
{
    print(currentPrefix() + line);
}

QString PythonConsole::currentPrefix()
{
    return d->currentStatement.isEmpty() ? d->promptPrefix : d->continuationPrefix;
}

void PythonConsole::appendCurrentStatement(QString line)
{
    d->currentStatement.append(line + "\n");
}

PythonObject PythonConsole::runCurrentStatement()
{
    PythonDict globals = PythonModule::get("__main__").getDict();
    PythonObject result = PyRun_String(qUtf8Printable(d->currentStatement), Py_single_input, *globals, *globals);
    MEDPYTHON_CHECK_ERROR();
    return result;
}

void PythonConsole::handleEnteredLine(QString line)
{
    if (line.trimmed().isEmpty())
    {
        d->statementFinished = true;
        printLine();
    }
    else
    {
        appendCurrentStatement(line);
        printLine(line);

        try
        {
            runCurrentStatement();
        }
        catch (PythonIndentationError&)
        {
        }
        catch (PythonExceptionBase& exception)
        {
            print(exception.what());
        }
    }

    updateCommandLine();
}

void PythonConsole::print(QString text)
{
    d->outputText->moveCursor(QTextCursor::End);
    d->outputText->insertPlainText(text);
    QScrollBar* scrollBar = d->outputText->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

}
