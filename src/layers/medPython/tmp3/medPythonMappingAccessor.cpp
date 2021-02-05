#include "medPythonMappingAccessor.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{

MappingAccessor::MappingAccessor(const OBJECT& mapping, const OBJECT& key) :
    mapping(mapping), key(key)
{
}

PyObject* MappingAccessor::getReference() const
{
    // We store the value becaue, depending on the mapping type, the returned reference might be the only existing
    // reference, and so we must keep it alive for the duration of the access.
    PyObject* reference = coreFunction<PyObject*, &PyObject_GetItem, PyObject*, PyObject*>(*mapping, *key);
    OBJECT::setReference(reference);
    return reference;
}

void MappingAccessor::setReference(PyObject* reference) const
{
    OBJECT::setReference(reference);
    coreFunctionNoReturn<&PyObject_SetItem, PyObject*, PyObject*, PyObject*>(*mapping, *key, reference);
}

}
}
