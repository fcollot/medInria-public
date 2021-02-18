#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QList>

#include "medPythonStandardObjects.h"

namespace med::python
{

bool isModule(const Object& object);
bool isPackage(const Object& object);

Object createModule(QString name);
Object createPackage(QString name);
Object createSubModule(const Object& package, QString name);

Object getModule(QString name);

void runCodeInModuleContext(const Object& module, QString pythonCode);

QString moduleName(const Object& module);
Object moduleDict(const Object& module);

Object import(QString name, QList<QString> fromList = {}, const Object& globals = dict(), bool relative = false);
Object import(const Object& importingModule, QString name, QList<QString> fromList = {}, bool relative = false);

Object retrieveModuleObjects(const Object& sourceModule, QList<QString> fromList);

QList<QString> publicModuleAttributes(const Object& module);

} // namespace med::python
