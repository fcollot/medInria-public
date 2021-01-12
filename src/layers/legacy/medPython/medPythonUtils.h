#pragma once

#include <QString>

#include "medPython.h"
#include "medPythonExport.h"

#define TO_CSTRING(qstring) qUtf8Printable(qstring)

class MEDPYTHON_EXPORT medPythonUtils
{
public:
    static void runCode(QString pythonCode, PyObject* globals = nullptr, PyObject** output = nullptr);

    static void addSysPath(QString path);
    static void removeSysPath(QString path);

private:
    medPythonUtils() = default;
    ~medPythonUtils() = default;
};
