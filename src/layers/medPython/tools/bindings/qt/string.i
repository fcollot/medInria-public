%{
#include "medPythonError.h"
#include "medPythonQStringConversion.h"
%}

%typemap(typecheck) QString = char *;

%medPythonTypemaps(QString);

%apply QString { const QString };

%typemap(in) const QString& (QString temp)
{
    // in 2
    medPythonConvert($input, &temp);
    med::python::propagateErrorIfOccurred();
    $1 = &temp;
}

%typemap(directorout) const QString& (QString temp)
{
    // director out 2
    medPythonConvert($input, &temp);
    med::python::propagateErrorIfOccurred();
    $result = temp;
}

%typemap(out) const QString&
{
    // out 2
    medPythonConvert(*$1, &$result);
    med::python::propagateErrorIfOccurred();
}

%typemap(directorin) const QString& (PyObject* temp)
{
    // director in 2
    medPythonConvert($1, &temp);
    med::python::propagateErrorIfOccurred();
    $input = temp;
}
