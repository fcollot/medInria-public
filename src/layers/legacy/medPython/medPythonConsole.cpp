#include "medPythonConsole.h"

#include <QCloseEvent>
#include <QTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>

#include "medPythonConsoleCommandLine.h"
#include "medPythonError.h"
#include "medPythonModuleHandle.h"
#include "medPythonOutputStream.h"
#include "medPythonTypeUtils.h"

class medPythonConsolePrivate
{
public:
    QTextEdit* outputLines;
    medPythonConsoleCommandLine* commandLine;
    QString currentCommand;
};

medPythonConsole::medPythonConsole(QWidget* parent) :
    QWidget(parent), d(new medPythonConsolePrivate)
{
    initializeGUI();
}

medPythonConsole::~medPythonConsole()
{
    delete d;
}

void medPythonConsole::listenTo(medPythonOutputStream* stream)
{
    connect(stream, &medPythonOutputStream::recieved, this, &medPythonConsole::print);
}

void medPythonConsole::showEvent(QShowEvent* event)
{
    d->commandLine->setFocus();
    event->accept();
}

void medPythonConsole::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void medPythonConsole::initializeGUI()
{
    setWindowTitle("Python console");

    setLayout(new QVBoxLayout);
    resize(800, 600);

    d->outputLines = new QTextEdit;
    layout()->addWidget(d->outputLines);
    d->outputLines->setReadOnly(true);
    d->outputLines->setUndoRedoEnabled(false);

    d->commandLine = new medPythonConsoleCommandLine;
    layout()->addWidget(d->commandLine);
    connect(d->commandLine, &QLineEdit::returnPressed, this, &medPythonConsole::handleStatement);

    QTextDocument *textDocument = d->outputLines->document();
    QFont font = textDocument->defaultFont();
    font.setFamily("Courier");
    textDocument->setDefaultFont(font);
    d->commandLine->setFont(font);
}

void medPythonConsole::handleStatement()
{
    PyErr_Clear();
    QString statement = d->commandLine->text();
    d->commandLine->clear();
    d->currentCommand.append(statement);
    print(statement);

    PyObject* globals = PyModule_GetDict(PyImport_AddModule("__main__"));
    PyObject* result = PyRun_String(qUtf8Printable(d->currentCommand), Py_single_input, globals, globals);
    Py_XDECREF(result);

    if (PyErr_Occurred())
    {
        if (PyErr_ExceptionMatches(PyExc_IndentationError) && !d->possibleMultiLineCommand)
        {
            d->c
        }
        else
        {
            d->multiLineCommand.clear();
            print(medPythonError::getErrorMessage());
        }

        medPythonError::clearError();
    }
}

void medPythonConsole::printCommand(QString text)
{
    d->outputLines->setFontWeight(QFont::Bold);
    print(QString(">>> ") + text + "\n");
    d->outputLines->setFontWeight(QFont::Normal);
}

void medPythonConsole::print(QString text)
{
    d->outputLines->moveCursor(QTextCursor::End);
    d->outputLines->insertPlainText(text);
    QScrollBar* scrollBar = d->outputLines->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}
