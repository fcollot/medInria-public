#pragma once

#include <QString>

#include "medPythonError.h"

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

}
}
