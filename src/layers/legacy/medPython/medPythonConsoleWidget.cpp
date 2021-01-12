#include "medPythonConsoleWidget.h"

#include <QCloseEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "medPythonError.h"
#include "medPythonModuleHandle.h"
#include "medPythonTypeUtils.h"

class medPythonConsoleWidgetPrivate
{
public:
    QTextEdit* outputLines;
    QLineEdit* commandLine;
};

medPythonConsoleWidget::medPythonConsoleWidget() :
    d(new medPythonConsoleWidgetPrivate)
{
    initializeGUI();
}

medPythonConsoleWidget::~medPythonConsoleWidget()
{
    delete d;
}

void medPythonConsoleWidget::initializeGUI()
{
    setLayout(new QVBoxLayout);
    resize(800, 600);

    d->outputLines = new QTextEdit;
    layout()->addWidget(d->outputLines);
    d->outputLines->setReadOnly(true);

    d->commandLine = new QLineEdit;
    layout()->addWidget(d->commandLine);
    connect(d->commandLine, &QLineEdit::returnPressed, this, &medPythonConsoleWidget::handleStatement);
}

void medPythonConsoleWidget::handleStatement()
{
    PyErr_Clear();

    QString statement = d->commandLine->text();
    d->commandLine->clear();
    QString formattedStatement = QString(">>> ") + statement;
    print(formattedStatement);

    PyObject* globals = PyModule_GetDict(PyImport_AddModule("__main__"));
    PyObject* result = PyRun_String(qUtf8Printable(statement), Py_single_input, globals, globals);
    Py_XDECREF(result);

    if (PyErr_Occurred())
    {

    }
}

void medPythonConsoleWidget::print(QString text)
{
    d->outputLines->append(text);
}


void medPythonConsoleWidget::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}
