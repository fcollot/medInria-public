#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

// Resources are generally compiled into the executable or library binaries
// using Qt's resources system. Sometimes though it is necessary (or preferable)
// for resources to be placed in seperate files. This can be done through the
// add_external_resources macro of the cmake project. External resources are
// stored differently depending on the OS, so this file provides a function to
// retrieve the path of the resource in a platform-independant way.
// NOTE: External Qt binary resources (rcc files) must also be registered using
// QResource::registerResource(path_to_resource).


#include <QString>

namespace med
{

// Returns the path of an external resource. If the resource was added through
// a library target then the library name must be provided. A null string is
// returned if the resource does not exist.
QString getExternalResourcePath(QString filename, QString libraryName = QString());

} // namespace med
