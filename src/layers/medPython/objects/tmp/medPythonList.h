#pragma once

#include "medPythonExport.h"
#include "medPythonContainer.h"

namespace med
{
namespace python
{

class MEDPYTHON_EXPORT List : public Container
{
public:
    MappingAccessor operator[](const ARG& key);
};

} // namespace python
} // namespace med
