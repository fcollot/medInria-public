#include "medPythonSequence.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{

SequenceAccessor Sequence::operator[](ssize_t index)
{
    return QSharedPointer<SequenceAccessor>(new SequenceAccessor(*this, index));
}

} // namespace python
} // namespace med
