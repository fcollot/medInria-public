#ifndef MEDABSTRACTDATAWRITER
#define MEDABSTRACTDATAWRITER

#include <dtkCore/dtkAbstractDataWriter.h>
#include <medCoreExport.h>


class medAbstractData;
/**
 * Extending dtkAbstractDataWriter class to allow the export of a list of data
 */
class MEDCORE_EXPORT medAbstractDataWriter : public dtkAbstractDataWriter
{
    Q_OBJECT

public:
    using dtkAbstractDataWriter::setData;
    virtual void setData(QList<medAbstractData*> data) {}
};


#endif // MEDABSTRACTDATAWRITER
