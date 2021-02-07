#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QString>
#include <QVariant>

#include "medPythonCoreForward.h"

namespace med
{
namespace python
{

bool convertToPython(bool value, PyObject** output);
bool convertFromPython(PyObject* object, bool* output);

bool convertToPython(int value, PyObject** output);
bool convertFromPython(PyObject* object, int* output);

bool convertToPython(long value, PyObject** output);
bool convertFromPython(PyObject* object, long* output);

bool convertToPython(double value, PyObject** output);
bool convertFromPython(PyObject* object, double* output);

bool convertToPython(QString value, PyObject** output);
bool convertFromPython(PyObject* object, QString* output);

bool convertToPython(QVariant value, PyObject** output);
bool convertFromPython(PyObject* object, QVariant* output);

} // namespace python
} // namespace med
