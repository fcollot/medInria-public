#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med::python
{

class MEDPYTHON_EXPORT Module : public Object
{
public:
    // Returns the module object corresponding to the fully qualified 'name' if
    // the module has already been loaded, or else returns None (no exception
    // is thrown).
    static Module get(QString name);

    static Module main();

    static Module create(QString name, bool isPackage = false);
};

} // namespace med::python
