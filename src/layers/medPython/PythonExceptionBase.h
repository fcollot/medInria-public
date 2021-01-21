#pragma once

#include <stdexcept>

#include "PythonModule.h"
#include "PythonType.h"
#include "Utils.h"

namespace medPython
{

class PythonExceptionBasePrivate;

class PythonExceptionBase : public PythonObject, public std::exception
{
public:
    static PyObject* const nativeType;

    // for creating new exceptions from cpp
    static PythonExceptionBase create(SourceCodeLocation throwSite, PythonTuple args = nullptr, PythonDict kwargs = nullptr);

    PythonExceptionBase(SourceCodeLocation throwSite, PyObject* nativeException);
    PythonExceptionBase(const PythonExceptionBase& other);
    PythonExceptionBase& operator=(const PythonExceptionBase& other);
    ~PythonExceptionBase();

    const char* what() const throw() override;

private:
    PythonExceptionBasePrivate* const d;

    QString formatMessage();
    QString formatMessageWithTracebackModule(PythonModule& tracebackModule);
};

PyObject* const PythonExceptionBase::nativeType = PyExc_BaseException;

}
