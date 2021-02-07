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
#include "medPythonExport.h"

MEDPYTHON_EXPORT bool medPython_convert(bool value, PyObject** output);
MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, bool* output);

MEDPYTHON_EXPORT bool medPython_convert(int value, PyObject** output);
MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, long* output);

MEDPYTHON_EXPORT bool medPython_convert(double value, PyObject** output);
MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, double* output);
