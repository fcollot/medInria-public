#pragma once

#include "medPythonExport.h"

class MEDPYTHON_EXPORT medPython
{
public:
    static void initialize();

    ~medPython();

private:
    medPython();

    void initTracebackModule();
};
