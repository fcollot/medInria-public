#pragma once

#include <QString>

#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

Reference getType(const AbstractReference& object);
QString getTypeName(const AbstractReference& object);

bool hasAttribute(const AbstractReference& object, QString name);
Reference getAttribute(const AbstractReference& object, QString name);
void setAttribute(AbstractReference& object, QString name, const AbstractReference& value);

QList<QString> dir(const AbstractReference& object);

bool isTrue(const AbstractReference& object);
bool isNot(const AbstractReference& object);

Reference none();

}
}
}
