#pragma once

// create here ?
#define CHECK_PYTHON_ERROR(checker) checker.check(__FILE__, __LINE__, __func__)

#include "medPythonError.h"
#include "medPythonExport.h"
#include "medPythonUtils.h"

class medPythonErrorCheckerPrivate;

class MEDPYTHON_EXPORT medPythonErrorChecker
{
public:
    medPythonErrorChecker();

    void clear();
    void check(QString file, int line, QString function);

private:
    medPythonErrorCheckerPrivate* const d;

    QString getPythonErrorMessage();
    QString formatError(PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback);
};
