#include "medPythonOutputStream.h"

namespace med
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
