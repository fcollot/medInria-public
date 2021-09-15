%{
#include <dtkCoreSupport/dtkAbstractData.h>
#include <dtkCoreSupport/dtkAbstractObject.h>
#include <dtkCoreSupport/dtkSmartPointer.h>

#include <medAbstractData.h>
#include <medAbstractImageData.h>
#include <medAbstractMeshData.h>
#include <medDataIndex.h>
#include <medDataManager.h>
%}

%inline
%{
void debug(QString message)
{
    qDebug() << message;
}
%}

%rename(DataIndex) medDataIndex;
%include "medDataIndex.h"

%qListTypemaps(medDataIndex, SWIG_TYPECHECK_OBJECT_ARRAY)

%ignore dtkAbstractData::draw;
%ignore dtkAbstractData::output;
%ignore dtkAbstractData::parameter;
%ignore dtkAbstractData::setParameter;
%ignore dtkAbstractData::update;
%ignore dtkAbstractData::thumbnail;
%ignore dtkAbstractData::thumbnails;
%ignore dtkAbstractData::toVariant;
%ignore dtkAbstractData::fromVariant;

%feature("ref") dtkAbstractObject "$this->retain();";
%feature("unref") dtkAbstractObject "$this->release();";

class dtkAbstractObject : public QObject
{
Q_OBJECT

public:
    virtual QString description() const;
    virtual QString identifier() const;
    virtual QString name() const;

    int count() const;
    int retain() const;
    int release() const;

    void enableDeferredDeletion(bool value);
    bool isDeferredDeletionEnabled() const;

    bool hasProperty(const QString& key) const;
    void addProperty(const QString& key, const QStringList& values);
    void addProperty(const QString& key, const QString& value);
    void setProperty(const QString& key, const QString& value);
    QStringList propertyList() const;
    QStringList propertyValues(const QString& key) const;
    QString property(const QString& key) const;

    bool hasMetaData(const QString& key) const;
    void addMetaData(const QString& key, const QStringList& values);
    void addMetaData(const QString& key, const QString& value);
    void setMetaData(const QString& key, const QStringList& values);
    void setMetaData(const QString& key, const QString& value);
    QStringList metaDataList() const;
    QStringList metaDataValues(const QString& key) const;
    QString metadata(const QString& key) const;
    QStringList metadatas(const QString& key) const;
    void copyMetaDataFrom(const dtkAbstractObject *obj);
};

%include "dtkCoreSupport/dtkSmartPointer.h"
%include "dtkCoreSupport/dtkAbstractData.h"

%rename(AbstractData) medAbstractData;
%include "medAbstractData.h"

%rename(AbstractMeshData) medAbstractMeshData;
%include "medAbstractMeshData.h"

%rename(AbstractImageData) medAbstractImageData;
%include "medAbstractImageData.h"

// Functions accepting raw medAbstractData pointers must also accept smart pointers, and vice versa.

%fragment("typecheck"{medAbstractData*}, "header")
{
    int medAbstractData_Check(PyObject* input)
    {
        if (!SWIG_IsOK(SWIG_ConvertPtr(input, nullptr, $descriptor(medAbstractData*), 0)))
        {
            if (!SWIG_IsOK(SWIG_ConvertPtr(input, nullptr, $descriptor(dtkSmartPointer<medAbstractData>*), 0)))
            {
                return false;
            }
        }

        return true;
    }
}

%typecheck(3000, fragment = "typecheck"{medAbstractData*}) medAbstractData*
{
    $1 = medAbstractData_Check($input) ? 1 : 0;
}

%typecheck(3001, fragment = "typecheck"{medAbstractData*}) dtkSmartPointer<medAbstractData>
{
    $1 = medAbstractData_Check($input) ? 1 : 0;
}

%typemap(out) medAbstractData*
{
    if ($1)
    {
        medAbstractMeshData* meshData = dynamic_cast<medAbstractMeshData*>($1);

        if (meshData)
        {
            $result = SWIG_NewPointerObj(meshData, $descriptor(medAbstractMeshData*), 1);
        }
        else
        {
            medAbstractImageData* imageData = dynamic_cast<medAbstractImageData*>($1);

            if (imageData)
            {
                $result = SWIG_NewPointerObj(imageData, $descriptor(medAbstractImageData*), 1);
            }
            else
            {
                $result = SWIG_NewPointerObj($1, $1_descriptor, 1);
            }
        }

        $1->retain();
    }
    else
    {
        $result = Py_None;
        Py_INCREF($result);
    }
}

%rename(DataManager) medDataManager;
%include "medDataManager.h"
