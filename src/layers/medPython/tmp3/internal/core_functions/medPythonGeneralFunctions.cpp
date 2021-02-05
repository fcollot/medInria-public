#include "medPythonGeneralFunctions.h"

#include "medPythonContainerConversions.h"
#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{
namespace internal
{

Reference getType(const AbstractReference& object)
{
    return coreFunction<PyObject*, &PyObject_Type, PyObject*>(object.get());
}

QString getTypeName(const AbstractReference& object)
{
    return Py_TYPE(object.get())->tp_name;
}

bool hasAttribute(const AbstractReference& object, QString name)
{
    return coreFunction<int, &PyObject_HasAttrString, PyObject*, const char*>(object.get(), qUtf8Printable(name));
}

Reference getAttribute(const AbstractReference& object, QString name)
{
    return coreFunction<PyObject*, &PyObject_GetAttrString, PyObject*, const char*>(object.get(), qUtf8Printable(name));
}

void setAttribute(AbstractReference& object, QString name, const AbstractReference& value)
{
    coreFunctionNoReturn<&PyObject_SetAttrString, PyObject*, const char*, PyObject*>(object.get(), qUtf8Printable(name), value.get());
}

QList<QString> dir(const AbstractReference& object)
{
    Reference dir = coreFunction<PyObject*, &PyObject_Dir, PyObject*>(object.get());
    return Convert<QList<QString> >::fromPython(dir);
}

Reference none()
{
    return Reference::borrowed(Py_None);
}

}
}
}
