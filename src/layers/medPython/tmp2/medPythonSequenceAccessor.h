#pragma once

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med
{

class PythonSequenceAccessorPrivate;

class MEDPYTHON_EXPORT PythonSequenceAccessor : public PythonObjectBase
{
public:
    PythonSequenceAccessor(PythonObjectBase& sequence, size_t index);
    virtual ~PythonSequenceAccessor();

    PythonObjectBase& operator=(PythonObjectBase& other);

private:
    PythonSequenceAccessorPrivate* const d;
};

}
