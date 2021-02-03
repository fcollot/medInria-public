#pragma once

#include "QString"

#include "medPythonReference.h"

#define MEDPYTHON_CODE_LOCATION med::python::internal::SourceCodeLocation{__FILE__, __LINE__, __func__}

namespace med
{
namespace python
{
namespace internal
{

struct SourceCodeLocation
{
    QString file;
    int line;
    QString function;
};

void initializeErrorPropagation();

bool errorOccured();

void clearError();

Reference retrieveException();

void propagateErrorIfOccured(const SourceCodeLocation& propagationSite);

void propagateError(const SourceCodeLocation& propagationSite, const Reference& nativeException);

}
}
}
