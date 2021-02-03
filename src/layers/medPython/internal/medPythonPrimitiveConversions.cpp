#include "medPythonPrimitiveConversions.h"

#include "medPythonCoreWrappers.h"
#include "medPythonGeneralFunctions.h"

namespace med
{
namespace python
{
namespace internal
{

Reference Convert<bool>::toPython(bool value)
{
    return coreFunction<PyObject*, &PyBool_FromLong, const long&>(value);
}

bool Convert<bool>::fromPython(const AbstractReference& object)
{
    return isTrue(object);
}

Reference Convert<long>::toPython(long number)
{
    return coreFunction<PyObject*, &PyLong_FromLong, long&>(number);
}

long Convert<long>::fromPython(const AbstractReference& object)
{
    return coreFunction<long, &PyLong_AsLong, PyObject*>(object.get());
}

Reference Convert<double>::toPython(double number)
{
    return coreFunction<PyObject*, &PyFloat_FromDouble, double&>(number);
}

double Convert<double>::fromPython(const AbstractReference& object)
{
    return coreFunction<double, &PyFloat_AsDouble, PyObject*>(object.get());
}

Reference Convert<QString>::toPython(QString string)
{
    return coreFunction<PyObject*, &PyUnicode_FromString, const char*>(qUtf8Printable(string));
}

QString Convert<QString>::fromPython(const AbstractReference& object)
{
    PyObject* nativeOject = object.get();
    Reference stringAsPyBytes;

    if (PyBytes_Check(nativeOject))
    {
        stringAsPyBytes = object;
    }
    else if (PyUnicode_Check(nativeOject))
    {
        stringAsPyBytes = coreFunction<PyObject*, &PyUnicode_AsEncodedString, PyObject*, const char*, const char*>
                (nativeOject, "UTF-8", "strict");
    }

    if (stringAsPyBytes)
    {
        return strdup(PyBytes_AS_STRING(stringAsPyBytes.get()));
    }
    else
    {
        // The object is not a string, so apply str() and retry
        Reference stringObject = coreFunction<PyObject*, &PyObject_Str, PyObject*>(nativeOject);
        return fromPython(stringObject);
    }
}

}
}
}
