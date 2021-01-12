%{
#include <QString>
#include "medPythonTypeUtils.h"
%}

%typemap(typecheck) QString = char *;

%typemap(in) QString
{
    $1 = QString(medPythonTypeUtils::fromString($input));
}

%typemap(directorout) QString
{
    $result = QString(medPythonTypeUtils::fromString($input));
}

%typemap(out) QString
{
    $result = medPythonTypeUtils::toPython($1);
}

%typemap(directorin) QString
{
    $input = medPythonTypeUtils::toPython($1);
}

%apply QString { const QString }
