#pragma once

#include <QString>

#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

Reference createList(size_t size = 0);
Reference createTuple(size_t size = 0);
Reference createDict();

size_t getSize(const AbstractReference& container);

Reference mappingKeys(const AbstractReference& mapping);
Reference mappingValues(const AbstractReference& mapping);

Reference getItem(const AbstractReference& sequence, size_t index);
Reference getItem(const AbstractReference& mapping, const AbstractReference& key);
Reference getItem(const AbstractReference& mapping, QString key);

void setItem(AbstractReference& sequence, size_t index, const AbstractReference& value);
void setItem(AbstractReference& mapping, const AbstractReference& key, const AbstractReference& value);
void setItem(AbstractReference& mapping, QString key, const AbstractReference& value);

void append(AbstractReference& list, const AbstractReference& item);

void mergeDicts(AbstractReference& dict, const AbstractReference& otherDict);

}
}
}
