#pragma once

#include <QString>

#include "medPythonConvert.h"

namespace med
{
namespace python
{
namespace internal
{

template <>
class Convert<bool>
{
public:
    static Reference toPython(bool value);

    static bool fromPython(const AbstractReference& object);
};

template <>
class Convert<long>
{
public:
    static Reference toPython(long number);

    static long fromPython(const AbstractReference& object);
};

template <>
class Convert<double>
{
public:
    static Reference toPython(double number);

    static double fromPython(const AbstractReference& object);
};

template <>
class Convert<QString>
{
public:
    static Reference toPython(QString string);

    static QString fromPython(const AbstractReference& object);
};

}
}
}
