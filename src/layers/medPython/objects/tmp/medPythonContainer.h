#pragma once

#include "medPythonExport.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{

class MEDPYTHON_EXPORT Container : public Reference
{
public:
    virtual bool contains(const AbstractObject& item) const;
};

} // namespace python
} // namespace med
