/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Alexandre <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "itkFilters.h"
#include "itkFiltersPlugin.h"
#include "itkFiltersToolBox.h"
#include "itkFiltersDefinitions.h"

#include <dtkCore/dtkLog.h>

class itkFiltersPluginPrivate
{
public:
};

itkFiltersPlugin::itkFiltersPlugin ( QObject* parent ) : dtkPlugin ( parent ), d ( new itkFiltersPluginPrivate )
{

}

itkFiltersPlugin::~itkFiltersPlugin ( void )
{
    delete d;

    d = NULL;
}

bool itkFiltersPlugin::initialize ( void )
{
    if ( !itkFilters::registered() )
        dtkWarning() << "Unable to register itkFilters type";

    if ( !itkFiltersToolBox::registered() )
        dtkWarning() << "Unable to register itkFilters toolbox";

    return true;
}

bool itkFiltersPlugin::uninitialize ( void )
{
    return true;
}

QString itkFiltersPlugin::description ( void ) const
{
  QString description;
  description = "This plugin implements some basic filters of ITK:      \
                 add constant, subtract constant, multiply by constant, \
                 divide by constant, gaussian filter, normalize filter, \
                 median filter, invert filter, shrink filter and intensity filter";
  return description;
}

QString itkFiltersPlugin::name ( void ) const
{
    return "itkFiltersPlugin";
}

QString itkFiltersPlugin::contact(void) const
{
    return QString::fromUtf8("Alexandre Abadie <Alexandre.Abadie@inria.fr>");
}

QString itkFiltersPlugin::version(void) const
{
    return PLUGIN_VERSION;
}

QStringList itkFiltersPlugin::authors ( void ) const
{
    return QStringList() << "Alexandre Abadie <Alexandre.Abadie@inria.fr>";
}

QStringList itkFiltersPlugin::contributors ( void ) const
{
    return QStringList() << QString::fromUtf8("Benoit Bleuzé <Benoit.Bleuze@inria.fr>");
}

QStringList itkFiltersPlugin::dependencies ( void ) const
{
    return dtkPlugin::dependencies();
}


QStringList itkFiltersPlugin::tags ( void ) const
{
    return QStringList() << "ITK" << "process" << "preprocessing" << "filtering";
}

QStringList itkFiltersPlugin::types ( void ) const
{
    return QStringList() << "itkFilters";
}

Q_EXPORT_PLUGIN2 ( itkFiltersPlugin, itkFiltersPlugin )
