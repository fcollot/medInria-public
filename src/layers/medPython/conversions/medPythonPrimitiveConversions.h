#pragma once

#include <QString>

#include "medPythonConvert.h"

namespace med
{
namespace python
{

template <>
class Convert<bool>
{
public:
    static PyObject* toPython(bool value);

    static bool fromPython(PyObject* object);
};

template <>
class Convert<long>
{
public:
    static PyObject* toPython(long number);

    static long fromPython(PyObject* object);
};

template <>
class Convert<double>
{
public:
    static PyObject* toPython(double number);

    static double fromPython(PyObject* object);
};

template <>
class Convert<QString>
{
public:
    static PyObject* toPython(QString string);

    static QString fromPython(PyObject* object);
};

}
}
