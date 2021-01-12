#pragma once

#define THROW_EXCEPTION(message) throw std::runtime_error(\
    QString(QString(__FILE__)\
    + ", line " + QString::number(__LINE__)\
    + " ("+ __func__ + ")"\
    + ": " + message)\
    .toStdString().c_str())

#define THROW_PYTHON_EXCEPTION() THROW_EXCEPTION(medPythonError::getPythonErrorMessage())

#include "medPythonExport.h"
#include "medPythonModuleHandle.h"

class MEDPYTHON_EXPORT medPythonError
{
public:
    static medPythonModuleHandle getTracebackModule();

private:
    static PyObject* getLoadedTracebackModule();
    static PyObject* loadTracebackModule();
};
