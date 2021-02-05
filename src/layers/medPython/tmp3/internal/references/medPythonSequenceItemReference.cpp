#include "medPythonSequenceItemReference.h"

#include "medPythonContainerFunctions.h"

namespace med
{
namespace python
{
namespace internal
{

SequenceItemReference::SequenceItemReference(const Reference& sequence, size_t index) :
    sequence(sequence), index(index)
{
}

SequenceItemReference& SequenceItemReference::operator=(const AbstractReference& other)
{
    setItem(sequence, index, other);
    return *this;
}

PyObject* SequenceItemReference::get() const
{
    // We store the value becaue, depending on the sequence type, the returned reference might be the only existing
    // reference, and so we must keep it alive for the duration of the access.
    value = getItem(sequence, index);
    return value.get();
}

}
}
}
