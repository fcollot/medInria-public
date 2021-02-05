#include "medPythonMappingAccessor.h"

#include "medPythonErrorManager.h"
#include "medPythonFunctionWrappers.h"
#include "medPythonObject.h"

namespace med
{

class PythonMappingAccessorPrivate
{
public:
    PythonObject mapping;
    PythonObject key;
};

PythonMappingAccessor::PythonMappingAccessor(PythonObjectBase& mapping, PythonObjectBase& key) :
    PythonObjectBase(dynamic_cast<PythonObject&>(mapping)[key]), d(new PythonMappingAccessorPrivate)
{
    d->mapping = mapping;
    d->key = key;
}

PythonMappingAccessor::~PythonMappingAccessor()
{
    delete d;
}

PythonObjectBase& PythonMappingAccessor::operator=(PythonObjectBase& other)
{
    PythonFunctionWrappers::functionNoReturn<&PyObject_SetItem>(d->mapping, *d->key, *other);
    return PythonObjectBase::operator=(other);
}

}
