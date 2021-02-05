#pragma once

#include "medPythonError.h"
#include "medPythonUtils.h"

namespace med
{
namespace python
{

template <class OBJECT>
class MappingAccessor : public OBJECT
{
public:
    MappingAccessor(const OBJECT& mapping, const OBJECT& key) :
        mapping(mapping), key(key)
    {
    }

    MappingAccessor<OBJECT>& operator=(const OBJECT& other)
    {
        OBJECT::operator=(other);
        return *this;
    }

protected:
    PyObject* getReference() override
    {
        // We store the value becaue, depending on the mapping type, the returned reference might be the only existing
        // reference, and so we must keep it alive for the duration of the access.
        PyObject* reference = utils::getItem(*mapping, *key);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
        OBJECT::setReference(reference);
        return reference;
    }

    void setReference(PyObject* reference) override
    {
        OBJECT::setReference(reference);
        utils::setItem(*mapping, *key, reference);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
    }

private:
    OBJECT mapping;
    OBJECT key;
};

}
}
