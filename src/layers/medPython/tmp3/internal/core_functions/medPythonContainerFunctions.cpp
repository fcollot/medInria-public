#include "medPythonContainerFunctions.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{
namespace internal
{

Reference createList(size_t size)
{
    return coreFunction<PyObject*, &PyList_New, size_t&>(size);
}

Reference createTuple(size_t size)
{
    return coreFunction<PyObject*, &PyTuple_New, size_t&>(size);
}

Reference createDict()
{
    return coreFunction<PyObject*, &PyDict_New>();
}

size_t getSize(const AbstractReference& container)
{
    return coreFunction<size_t, &PyObject_Size, PyObject*>(container.get());
}

Reference mappingKeys(const AbstractReference& mapping)
{
    return coreFunction<PyObject*, &PyMapping_Keys, PyObject*>(mapping.get());
}

Reference mappingValues(const AbstractReference& mapping)
{
    return coreFunction<PyObject*, &PyMapping_Values, PyObject*>(mapping.get());
}

Reference getItem(const AbstractReference& sequence, size_t index)
{
    return coreFunction<PyObject*, &PySequence_GetItem, PyObject*, size_t&>(sequence.get(), index);
}

Reference getItem(const AbstractReference& mapping, const AbstractReference& key)
{
    return coreFunction<PyObject*, &PyObject_GetItem, PyObject*, PyObject*>(mapping.get(), key.get());
}

Reference getItem(const AbstractReference& mapping, QString key)
{
    return coreFunction<PyObject*, &PyMapping_GetItemString, PyObject*, const char*>(mapping.get(), qUtf8Printable(key));
}

void setItem(AbstractReference& sequence, size_t index, const AbstractReference& value)
{
    coreFunctionNoReturn<&PySequence_SetItem, PyObject*, size_t&, PyObject*>(sequence.get(), index, value.get());
}

void setItem(AbstractReference& mapping, const AbstractReference& key, const AbstractReference& value)
{
    coreFunctionNoReturn<&PyObject_SetItem, PyObject*, PyObject*, PyObject*>(mapping.get(), key.get(), value.get());
}

void setItem(AbstractReference& mapping, QString key, const AbstractReference& value)
{
    coreFunctionNoReturn<&PyMapping_SetItemString, PyObject*, const char*, PyObject*>(mapping.get(), qUtf8Printable(key), value.get());
}

void append(AbstractReference& list, const AbstractReference& item)
{
    coreFunctionNoReturn<&PyList_Append, PyObject*, PyObject*>(list.get(), item.get());
}

void mergeDicts(AbstractReference& dict, const AbstractReference& otherDict)
{
    coreFunctionNoReturn<&PyDict_Merge, PyObject*, PyObject*, const int&>(dict.get(), otherDict.get(), 1);
}

}
}
}
