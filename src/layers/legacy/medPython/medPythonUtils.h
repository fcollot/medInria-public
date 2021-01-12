#pragma once

#include <QString>

#undef slots
#define slots _slots

#undef _LARGEFILE_SOURCE

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#undef slots
#define slots Q_SLOTS

#include "medPythonExport.h"

class MEDPYTHON_EXPORT medPythonUtils
{
public:
    static void runPythonString(QString pythonString, PyObject* globals = nullptr, PyObject** output = nullptr);

    static void addSysPath(QString path);
    static void removeSysPath(QString path);

private:
    medPythonUtils() = default;
    ~medPythonUtils() = default;
};
