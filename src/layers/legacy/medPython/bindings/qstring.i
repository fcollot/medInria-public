%module qstring

%{
#include <QString>
#include "medPythonUtils.h"
%}

%import "utils.i"

%fragment("convertToPython"{QString}, "header")
{
    PyObject* convertToPython(QString input)
    {
        PyObject* output = PyString_FromString(input.toStdString().c_str());

        if (!output)
        {
            medPythonUtils::cppToPythonConversionError("QString");
        }

        return output;
    }
}

%fragment("convertToPython"{QString});

%typemap(typecheck) QString = char *;

%typemap(in) QString
{
    $1 = QString(convertPythonStringToQString($input));
}

%typemap(directorout) QString
{
    $result = QString(convertPythonStringToQString($input));
}

%typemap(out, fragment = "convertToPython"{QString}) QString
{
    $result = convertToPython($1);
}

%typemap(directorin, fragment = "convertToPython"{QString}) QString
{
    $input = convertToPython($1);
}

%apply QString { const QString }
