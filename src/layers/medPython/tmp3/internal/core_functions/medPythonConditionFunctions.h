#pragma once

#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

bool isTrue(const AbstractReference& object);

bool isNot(const AbstractReference& object);

bool equals(const AbstractReference& object, const AbstractReference& other);

bool notEquals(const AbstractReference& object, const AbstractReference& other);

bool lessThan(const AbstractReference& object, const AbstractReference& other);

bool greaterThan(const AbstractReference& object, const AbstractReference& other);

bool lessThanEquals(const AbstractReference& object, const AbstractReference& other);

bool greaterThanEquals(const AbstractReference& object, const AbstractReference& other);

}
}
}
