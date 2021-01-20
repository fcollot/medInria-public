%{
#include <QString>
#include "Conversion.h"
%}

%typemap(typecheck) QString = char *;

%typemap(in) QString
{
    $1 = QString(Conversion::fromString($input));
}

%typemap(directorout) QString
{
    $result = QString(Conversion::fromString($input));
}

%typemap(out) QString
{
    $result = Conversion::toPython($1);
}

%typemap(directorin) QString
{
    $input = Conversion::toPython($1);
}

%apply QString { const QString }


%typemap(in) const QString& (QString temp)
{
    temp = QString(Conversion::fromString($input));
    $1 = &temp;
}

%typemap(directorout) const QString& (QString temp)
{
    temp = QString(Conversion::fromString($input));
    $result = &temp;
}

%typemap(out) const QString&
{
    $result = Conversion::toPython($1);
}

%typemap(directorin) const QString&
{
    $input = Conversion::toPython($1);
}
