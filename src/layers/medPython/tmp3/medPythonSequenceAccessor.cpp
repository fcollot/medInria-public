#include "medPythonSequenceAccessor.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{

SequenceAccessor::SequenceAccessor(const OBJECT& sequence, size_t index) :
    sequence(sequence), index(index)
{
}

PyObject* SequenceAccessor::getReference() const
{
    // We store the value becaue, depending on the mapping type, the returned reference might be the only existing
    // reference, and so we must keep it alive for the duration of the access.
    PyObject* reference = coreFunction<PyObject*, &PySequence_GetItem, PyObject*, size_t>(*sequence, index);
    OBJECT::setReference(reference);
    return reference;
}

void SequenceAccessor::setReference(PyObject* reference) const
{
    OBJECT::setReference(reference);
    coreFunctionNoReturn<&PySequence_SetItem, PyObject*, size_t, PyObject*>(*sequence, index, reference);
}

}
}
