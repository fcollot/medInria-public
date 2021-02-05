#pragma once

#include "medPythonError.h"
#include "medPythonUtils.h"

namespace med
{
namespace python
{

template <class OBJECT>
class SequenceAccessor : public OBJECT
{
public:
    SequenceAccessor(const OBJECT& sequence, ssize_t index) :
        sequence(sequence), index(index)
    {
    }

    SequenceAccessor<OBJECT>& operator=(const OBJECT& other)
    {
        OBJECT::operator=(other);
        return *this;
    }

protected:
    PyObject* getReference() override
    {
        // We store the value becaue, depending on the mapping type, the returned reference might be the only existing
        // reference, and so we must keep it alive for the duration of the access.
        PyObject* reference = utils::getItem(*sequence, index);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
        OBJECT::setReference(reference);
        return reference;
    }

    void setReference(PyObject* reference) override
    {
        OBJECT::setReference(reference);
        utils::setItem(*sequence, index, reference);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
    }

private:
    OBJECT sequence;
    ssize_t index;
};

}
}
