#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <dtkCoreSupport/dtkSmartPointer>

#include <dtkCoreSupport/dtkAbstractDataConverter>
#include <dtkCoreSupport/dtkAbstractDataDeserializer>
#include <dtkCoreSupport/dtkAbstractDataReader>
#include <dtkCoreSupport/dtkAbstractDataSerializer>
#include <dtkCoreSupport/dtkAbstractDataWriter>

#include <medAbstractData.h>

#include <medCoreLegacyExport.h>

typedef std::function<medAbstractData*()> medAbstractDataTypeCreator;
typedef std::function<dtkAbstractDataReader*()> medAbstractDataReaderTypeCreator;
typedef std::function<dtkAbstractDataWriter*()> medAbstractDataWriterTypeCreator;
typedef std::function<dtkAbstractDataConverter*()> medAbstractDataConverterTypeCreator;
typedef std::function<dtkAbstractDataSerializer*()> medAbstractDataSerializerTypeCreator;
typedef std::function<dtkAbstractDataDeserializer*()> medAbstractDataDeserializerTypeCreator;

class medAbstractDataFactoryPrivate;

class MEDCORELEGACY_EXPORT medAbstractDataFactory
{
public:
    static medAbstractDataFactory *instance();

    template <typename dataType>
    bool registerDataType()
    {
        return registerDataType(dataType::staticIdentifier(), []() { return new dataType; });
    }

    template <typename readerType>
    bool registerDataReaderType()
    {
        return registerDataReaderType(readerType::staticIdentifier(), readerType::handled(), []() { return new readerType; });
    }

    template <typename writerType>
    bool registerDataWriterType()
    {
        return registerDataWriterType(writerType::staticIdentifier(), writerType::handled(), []() { return new writerType; });
    }

    bool registerDataType(const QString& type, medAbstractDataTypeCreator creator);
    bool registerDataReaderType(const QString& type, const QStringList& handled, medAbstractDataReaderTypeCreator creator);
    bool registerDataWriterType(const QString& type, const QStringList& handled, medAbstractDataWriterTypeCreator creator);
    bool registerDataConverterType(const QString& type, const QStringList& fromTypes, const QString& toType, medAbstractDataConverterTypeCreator creator);
    bool registerDataSerializerType(const QString& type, const QStringList& handled, medAbstractDataSerializerTypeCreator creator);
    bool registerDataDeserializerType(const QString& type, const QStringList& handled, medAbstractDataDeserializerTypeCreator creator);

    QStringList creators() const;
    QStringList readers() const;
    QStringList writers() const;
    QStringList converters() const;
    QStringList serializers() const;
    QStringList deserializers() const;

    void setReaderPriorities(const QStringList& priorities);
    void setWriterPriorities(const QStringList& priorities);
    void setConverterPriorities(const QStringList& priorities);

    const QStringList& readerPriorities() const;
    const QStringList& writerPriorities() const;
    const QStringList& converterPriorities() const;

    medAbstractData* create(const QString& type);
    dtkSmartPointer<medAbstractData> createSmartPointer(const QString& type);

    dtkAbstractDataReader* reader(const QString& type);
    dtkSmartPointer<dtkAbstractDataReader> readerSmartPointer(const QString& type);

    dtkAbstractDataWriter* writer(const QString& type);
    dtkSmartPointer<dtkAbstractDataWriter> writerSmartPointer(const QString& type);

    dtkAbstractDataConverter* converter(const QString& type);
    dtkSmartPointer<dtkAbstractDataConverter> converterSmartPointer(const QString& type);

    dtkAbstractDataSerializer* serializer(const QString& type);
    dtkSmartPointer<dtkAbstractDataSerializer> serializerSmartPointer(const QString& type);

    dtkAbstractDataDeserializer* deserializer(const QString& type);
    dtkSmartPointer<dtkAbstractDataDeserializer> deserializerSmartPointer(const QString& type);

protected:
    medAbstractDataFactory();
    virtual ~medAbstractDataFactory();

private:
    static medAbstractDataFactory* s_instance;

    medAbstractDataFactoryPrivate* const d;
};
