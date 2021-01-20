#include "PythonOutputStream.h"

namespace medPython
{

void PythonOutputStream::write(QString text)
{
    emit recieved(text);
}

void PythonOutputStream::flush()
{
    emit flushed();
}

}
