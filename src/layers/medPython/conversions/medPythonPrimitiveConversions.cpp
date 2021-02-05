#include "medPythonPrimitiveConversions.h"

#include "medPythonCoreWrappers.h"
#include "medPythonObject.h"

namespace med
{
namespace python
{

PyObject* Convert<bool>::toPython(bool value)
{
    return coreFunction<PyObject*, &PyBool_FromLong, const long&>(value);
}

bool Convert<bool>::fromPython(PyObject* object)
{
    return coreFunction<bool, &PyObject_IsTrue, PyObject*>(object);
}

PyObject* Convert<long>::toPython(long number)
{
    return coreFunction<PyObject*, &PyLong_FromLong, long&>(number);
}

long Convert<long>::fromPython(PyObject* object)
{
    return coreFunction<long, &PyLong_AsLong, PyObject*>(object);
}

PyObject* Convert<double>::toPython(double number)
{
    return coreFunction<PyObject*, &PyFloat_FromDouble, double&>(number);
}

double Convert<double>::fromPython(PyObject* object)
{
    return coreFunction<double, &PyFloat_AsDouble, PyObject*>(object);
}

PyObject* Convert<QString>::toPython(QString string)
{
    return coreFunction<PyObject*, &PyUnicode_FromString, const char*>(qUtf8Printable(string));
}

QString Convert<QString>::fromPython(PyObject* object)
{
    Object stringAsPyBytes;

    if (PyBytes_Check(object))
    {
        stringAsPyBytes.borrow(object);
    }
    else if (PyUnicode_Check(object))
    {
        stringAsPyBytes = coreFunction<PyObject*, &PyUnicode_AsEncodedString, PyObject*, const char*, const char*>
                (object, "UTF-8", "strict");
    }

    if (stringAsPyBytes)
    {
        return strdup(PyBytes_AS_STRING(*stringAsPyBytes));
    }
    else
    {
        // The object is not a string, so apply str() and retry
        Object stringObject = coreFunction<PyObject*, &PyObject_Str, PyObject*>(object);
        return fromPython(*stringObject);
    }
}

}
}
