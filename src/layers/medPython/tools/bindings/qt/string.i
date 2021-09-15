%{
#include "medPythonError.h"
#include "medPythonQStringConversion.h"
%}

%typemap(typecheck) QString = char *;

%medPythonTypemaps(QString);

%apply QString { const QString };
%apply QString { const QString& };

%typemap(in) const QString& (QString temp)
{
    medPythonConvert($input, &temp);
    med::python::propagateErrorIfOccurred();
    $1 = &temp;
}

%typemap(directorout) const QString& (QString temp)
{
    medPythonConvert($input, &temp);
    med::python::propagateErrorIfOccurred();
    $result = &temp;
}

%typemap(out) const QString&
{
    medPythonConvert(*$1, &$result);
    med::python::propagateErrorIfOccurred();
}

%typemap(directorin) const QString&
{
    medPythonConvert(*$1, &$input);
    med::python::propagateErrorIfOccurred();
}
