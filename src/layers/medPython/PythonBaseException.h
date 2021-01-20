#pragma once

#include <stdexcept>

#include <QString>

#include "PythonModule.h"

namespace medPython
{

class PythonBaseExceptionPrivate;

class PythonBaseException : public std::exception
{
public:
    static const PyObject* pythonType;

    PythonBaseException();
    ~PythonBaseException();

    const char* what() const throw() override;

    void setMessage(QString message);
    void setPythonInstance(PythonObject pythonInstance);

private:
    PythonBaseExceptionPrivate* const d;

    void retrieveExceptionData(PythonObject& type, PythonObject& value, PythonObject& traceback);
    QString formatExceptionMessage(PythonObject type, PythonObject value, PythonObject traceback);

    PythonModule getTracebackModule();
};

}
