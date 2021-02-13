#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonCoreForward.h"

namespace med
{
namespace python
{

class AbstractObject;
class Object;

bool convertToPython(const AbstractObject& object, PyObject** output);
bool convertFromPython(const PyObject* nativeObject, Object* output);

} // namespace python
} // namespace med
