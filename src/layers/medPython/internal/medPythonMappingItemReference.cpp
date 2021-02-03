#include "medPythonMappingItemReference.h"

#include "medPythonContainerFunctions.h"

namespace med
{
namespace python
{
namespace internal
{

MappingItemReference::MappingItemReference(const Reference& mapping, const Reference& key) :
    mapping(mapping), key(key)
{
}

MappingItemReference& MappingItemReference::operator=(const AbstractReference& other)
{
    setItem(mapping, key, other);
    return *this;
}

PyObject* MappingItemReference::get() const
{
    // We store the value becaue, depending on the mapping type, the returned reference might be the only existing
    // reference, and so we must keep it alive for the duration of the access.
    value = getItem(mapping, key);
    return value.get();
}

}
}
}
