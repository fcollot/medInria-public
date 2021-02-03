#pragma once

#include <stdexcept>

#include "medPythonError.h"
#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med
{
namespace python
{

class ExceptionBasePrivate;

class MEDPYTHON_EXPORT ExceptionBase : public Object, public std::exception
{
public:
    static Reference nativeType();

    ExceptionBase(const SourceCodeLocation& throwSite, QString message);
    ExceptionBase(const ExceptionBase& other);
    ExceptionBase& operator=(const ExceptionBase& other);
    ~ExceptionBase();

    const char* what() const throw() override;

private:
    ExceptionBasePrivate* const d;

    ExceptionBase(const SourceCodeLocation& throwSite, const Reference& nativeException);

    QString formatMessage();
    QString formatMessageWithTracebackModule(Object& tracebackModule);
};

}
}
