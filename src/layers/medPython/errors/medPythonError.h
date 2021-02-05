#pragma once

#include "QString"

#include "medPythonUtils.h"

#define MEDPYTHON_CODE_LOCATION med::python::SourceCodeLocation{__FILE__, __LINE__, __func__}

namespace med
{
namespace python
{

struct SourceCodeLocation
{
    QString file;
    int line;
    QString function;
};

bool errorOccured();

void clearError();

PyObject* retrieveException();

void propagateErrorIfOccured(const SourceCodeLocation& propagationSite);

void propagateCurrentError(const SourceCodeLocation& propagationSite);

void propagateError(const SourceCodeLocation& propagationSite, PyObject* nativeException);

PyObject* getTracebackModule();

namespace internal
{

void initializeErrorHandling();
void finalizeErrorHandling();

}

}
}
