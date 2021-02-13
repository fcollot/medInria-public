#pragma once

#include "medPythonContainer.h"
#include "medPythonExport.h"
#include "medPythonSequenceAccessor.h"

namespace med
{
namespace python
{

class MEDPYTHON_EXPORT Sequence : public Container
{
public:
    SmartReference operator[](ssize_t index);
};

} // namespace python
} // namespace med
