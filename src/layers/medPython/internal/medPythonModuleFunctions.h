#pragma once

#include <QList>

#include "medPythonContainerFunctions.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

bool isModule(const AbstractReference& object);
bool isPackage(const AbstractReference& object);

Reference createModule(QString name);
Reference createPackage(QString name);
Reference createSubModule(const AbstractReference& package, QString name);

Reference getModule(QString name);

void runCodeInModuleContext(const AbstractReference& module, QString pythonCode);

QString moduleName(const AbstractReference& module);
Reference moduleDict(const AbstractReference& module);

Reference import(QString name, QList<QString> fromList, const AbstractReference& globals = createDict(), bool relative = false);
Reference import(const AbstractReference& importingModule, QString name, QList<QString> fromList, bool relative = false);

Reference retrieveModuleObjects(const AbstractReference& sourceModule, QList<QString> fromList);

QList<QString> publicModuleAttributes(const AbstractReference& module);

}
}
}
