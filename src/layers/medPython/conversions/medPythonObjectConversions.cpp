/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonObjectConversions.h"

#include "medPythonObject.h"

using namespace med::python;

bool medPython_convert(const AbstractObject& object, PyObject** output)
{
    *output = object.newReference();
    return true;
}

bool medPython_convert(const PyObject* nativeObject, Object* output)
{
    (*output).borrow(nativeObject);
    return true;
}
