#pragma once

#include <QString>

#define MEDPYTHON_CODE_LOCATION SourceCodeLocation{__FILE__, __LINE__, __func__}

namespace medPython
{

struct SourceCodeLocation
{
    QString file;
    int line;
    QString function;
};

}
