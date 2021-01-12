#include "medPythonOutputStream.h"

void medPythonOutputStream::write(QString text)
{
    emit recieved(text);
}

void medPythonOutputStream::flush()
{
    emit flushed();
}
