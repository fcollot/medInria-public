#include "medPythonSequenceAccessor.h"

#include "medPythonCore.h"
#include "medPythonCoreWrappers.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{

struct SequenceAccessorPrivate
{
    Reference sequence;
    ssize_t index;
};

SequenceAccessor::SequenceAccessor(const Reference& sequence, ssize_t index) :
    d(new SequenceAccessorPrivate)
{
    d->sequence = sequence;
    d->index = index;
}

PyObject* SequenceAccessor::getReference()
{
    // We store the value becaue, depending on the container type, the returned
    // reference might be the only existing reference, and so we must keep it
    // alive for the duration of the access.
    PyObject* nativeSequence = d->sequence.getNativeReference();
    PyObject* reference = coreFunction<PyObject*, &PySequence_GetItem, PyObject*, PyObject*>(nativeSequence, d->index);
    Reference::setReference(reference);
    return reference;
}

void SequenceAccessor::setReference(PyObject* reference)
{
    Reference::setReference(reference);
    PyObject* nativeSequence = d->sequence.getNativeReference();
    coreFunctionNoReturn<&PySequence_SetItem, PyObject*, PyObject*>(nativeSequence, d->index, reference);
}

} // namespace python
} // namespace med
