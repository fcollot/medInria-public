#pragma once

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med
{

class PythonMappingAccessorPrivate;

class MEDPYTHON_EXPORT PythonMappingAccessor : public PythonObjectBase
{
public:
    PythonMappingAccessor(PythonObjectBase& mapping, PythonObjectBase& key);
    virtual ~PythonMappingAccessor();

    PythonObjectBase& operator=(PythonObjectBase& other);

private:
    PythonMappingAccessorPrivate* const d;
};

}
