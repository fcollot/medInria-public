#include "medPythonSequenceAccessor.h"

#include "medPythonErrorManager.h"
#include "medPythonFunctionWrappers.h"
#include "medPythonObject.h"

namespace med
{

class PythonSequenceAccessorPrivate
{
public:
    PythonObject sequence;
    size_t index;
};

PythonSequenceAccessor::PythonSequenceAccessor(PythonObjectBase& sequence, size_t index) :
    PythonObjectBase(dynamic_cast<PythonObject&>(sequence)[index]), d(new PythonSequenceAccessorPrivate)
{
    d->sequence = sequence;
    d->index = index;
}

PythonSequenceAccessor::~PythonSequenceAccessor()
{
    delete d;
}

PythonObjectBase& PythonSequenceAccessor::operator=(PythonObjectBase& other)
{
    PythonFunctionWrappers::functionNoReturn<&PySequence_SetItem>(d->sequence, d->index, *other);
    return PythonObjectBase::operator=(other);
}

}
