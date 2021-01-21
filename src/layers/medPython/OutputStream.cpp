#include "OutputStream.h"

namespace medPython
{

void OutputStream::write(QString text)
{
    emit recieved(text);
}

void OutputStream::flush()
{
    emit flushed();
}

}
