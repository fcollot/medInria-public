#include "Console.h"

#include <QDebug>
#include <QCloseEvent>
#include <QTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>

#include "CommandLineBase.h"
#include "Convert.h"
#include "Exception.h"
#include "PythonModule.h"
#include "PythonOutputStream.h"

namespace medPython
{

class ConsoleBasePrivate
{
public:
    QTextEdit* outputLines;
    CommandLineBase* commandLine;
    QString currentStatement;
};

ConsoleBase::ConsoleBase(QWidget* parent) :
    QWidget(parent), d(new ConsoleBasePrivate)
{
    initializeGUI();
}

ConsoleBase::~ConsoleBase()
{
    delete d;
}

void ConsoleBase::listenTo(PythonOutputStream* stream)
{
    connect(stream, &PythonOutputStream::recieved, this, &ConsoleBase::print);
}

void ConsoleBase::showEvent(QShowEvent* event)
{
    d->commandLine->setFocus();
    event->accept();
}

void ConsoleBase::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void ConsoleBase::initializeGUI()
{
    setWindowTitle("Python console");

    setLayout(new QVBoxLayout);
    resize(800, 600);

    d->outputLines = new QTextEdit;
    layout()->addWidget(d->outputLines);
    d->outputLines->setReadOnly(true);
    d->outputLines->setUndoRedoEnabled(false);

    d->commandLine = createCommandLine();
    layout()->addWidget(d->commandLine);
    connect(d->commandLine, &ConsoleCommandLine::lineEntered, this, &ConsoleBase::handleEnteredLine);
    setDefaultCommandLinePrefix();

    QTextDocument *textDocument = d->outputLines->document();
    QFont font = textDocument->defaultFont();
    font.setFamily("Courier");
    textDocument->setDefaultFont(font);
    d->commandLine->setFont(font);

    int numVisibleLines = height()/fontMetrics().height();

    for (int i = 0; i < numVisibleLines; i++)
    {
        print("\n");
    }
}

void ConsoleBase::setDefaultCommandLinePrefix()
{
    d->commandLine->setPrefix(">>> ");
}

void ConsoleBase::handleEnteredLine(QString prefix, QString line)
{
    Error::clearError();

    print(prefix + line + "\n");
    bool lineIsEmpty = line.trimmed().isEmpty();
    bool multilineStatement = !d->currentStatement.isEmpty();
    d->currentStatement.append(line + "\n");

    if (!lineIsEmpty || multilineStatement)
    {
        PyObject* globals = PyModule_GetDict(PyImport_AddModule("__main__"));
        PyObject* result = PyRun_String(qUtf8Printable(d->currentStatement), Py_single_input, globals, globals);
        Py_XDECREF(result);
    }

    setDefaultCommandLinePrefix();

    if (Error::errorOccurred())
    {
        if (lineIsEmpty || !PyErr_ExceptionMatches(PyExc_IndentationError))
        {
            print(Error::getErrorMessage());
            d->currentStatement.clear();
        }
        else
        {
            d->commandLine->setPrefix("... ");
        }

        Error::clearError();
    }
    else
    {
        if (!multilineStatement || lineIsEmpty)
        {
            d->currentStatement.clear();
        }
        else
        {
            d->commandLine->setPrefix("... ");
        }
    }
}

void ConsoleBase::print(QString text)
{
    d->outputLines->moveCursor(QTextCursor::End);
    d->outputLines->insertPlainText(text);
    QScrollBar* scrollBar = d->outputLines->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

}
