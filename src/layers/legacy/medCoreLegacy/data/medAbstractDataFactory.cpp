/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractDataFactory.h>

namespace
{

struct medAbstractDataTypeInfo
{
    medAbstractDataTypeInfo() {}
    medAbstractDataTypeInfo(medAbstractDataTypeCreator creator) :
        creator(creator) {}

    medAbstractDataTypeCreator creator;
};

struct medAbstractDataReaderTypeInfo
{
    medAbstractDataReaderTypeInfo() {}
    medAbstractDataReaderTypeInfo(QStringList handled, medAbstractDataReaderTypeCreator creator) :
        handled(handled), creator(creator) {}

    QStringList handled;
    medAbstractDataReaderTypeCreator creator;
};

struct medAbstractDataWriterTypeInfo
{
    medAbstractDataWriterTypeInfo() {}
    medAbstractDataWriterTypeInfo(QStringList handled, medAbstractDataWriterTypeCreator creator) :
        handled(handled), creator(creator) {}

    QStringList handled;
    medAbstractDataWriterTypeCreator creator;
};

struct medAbstractDataConverterTypeInfo
{
    medAbstractDataConverterTypeInfo() {}
    medAbstractDataConverterTypeInfo(QStringList fromTypes, QString toType, medAbstractDataConverterTypeCreator creator) :
        fromTypes(fromTypes), toType(toType), creator(creator) {}

    QStringList fromTypes;
    QString toType;
    medAbstractDataConverterTypeCreator creator;
};

struct medAbstractDataSerializerTypeInfo
{
    medAbstractDataSerializerTypeInfo() {}
    medAbstractDataSerializerTypeInfo(QStringList handled, medAbstractDataSerializerTypeCreator creator) :
        handled(handled), creator(creator) {}

    QStringList handled;
    medAbstractDataSerializerTypeCreator creator;
};

struct medAbstractDataDeserializerTypeInfo
{
    medAbstractDataDeserializerTypeInfo() {}
    medAbstractDataDeserializerTypeInfo(QStringList handled, medAbstractDataDeserializerTypeCreator creator) :
        handled(handled), creator(creator) {}

    QStringList handled;
    medAbstractDataDeserializerTypeCreator creator;
};


typedef QHash<QString, medAbstractDataTypeInfo> medAbstractDataTypeHash;
typedef QHash<QString, medAbstractDataReaderTypeInfo> medAbstractDataReaderTypeHash;
typedef QHash<QString, medAbstractDataWriterTypeInfo> medAbstractDataWriterTypeHash;
typedef QHash<QString, medAbstractDataConverterTypeInfo> medAbstractDataConverterTypeHash;
typedef QHash<QString, medAbstractDataSerializerTypeInfo> medAbstractDataSerializerTypeHash;
typedef QHash<QString, medAbstractDataDeserializerTypeInfo> medAbstractDataDeserializerTypeHash;

} // namespace

class medAbstractDataFactoryPrivate
{
public:
    QHash<QString, medAbstractDataTypeInfo> dataTypes;
    QHash<QString, medAbstractDataReaderTypeInfo> readerTypes;
    QHash<QString, medAbstractDataWriterTypeInfo> writerTypes;
    QHash<QString, medAbstractDataConverterTypeInfo> converterTypes;
    QHash<QString, medAbstractDataSerializerTypeInfo> serializerTypes;
    QHash<QString, medAbstractDataDeserializerTypeInfo> deserializerTypes;

    QStringList readerPriorities;
    QStringList writerPriorities;
    QStringList converterPriorities;
};

medAbstractDataFactory *medAbstractDataFactory::s_instance = nullptr;

medAbstractDataFactory* medAbstractDataFactory::instance(void)
{
    if (!s_instance)
    {
        s_instance = new medAbstractDataFactory;
    }

    return s_instance;
}

medAbstractDataFactory::medAbstractDataFactory() : d(new medAbstractDataFactoryPrivate)
{
}

medAbstractDataFactory::~medAbstractDataFactory()
{
    delete d;
}

bool medAbstractDataFactory::registerDataType(const QString& type, medAbstractDataTypeCreator creator)
{
    bool success = false;

    if (!d->dataTypes.contains(type))
    {
        d->dataTypes.insert(type, creator);
        success = true;
    }

    return success;
}

bool medAbstractDataFactory::registerDataReaderType(const QString& type, const QStringList& handled, medAbstractDataReaderTypeCreator creator)
{
    bool success = false;

    if (!d->readerTypes.contains(type))
    {
        d->readerTypes.insert(type, medAbstractDataReaderTypeInfo(handled, creator));
        success = true;
    }

    return success;
}

bool medAbstractDataFactory::registerDataWriterType(const QString& type, const QStringList& handled, medAbstractDataWriterTypeCreator creator)
{
    bool success = false;

    if (!d->writerTypes.contains(type))
    {
        d->writerTypes.insert(type, medAbstractDataWriterTypeInfo(handled, creator));
        success = true;
    }

    return success;
}

bool medAbstractDataFactory::registerDataConverterType(const QString& type, const QStringList& fromTypes, const QString& toType, medAbstractDataConverterTypeCreator creator)
{
    bool success = false;

    if (!d->converterTypes.contains(type))
    {
        d->converterTypes.insert(type, medAbstractDataConverterTypeInfo(fromTypes, toType, creator));
        success = true;
    }

    return success;
}

bool medAbstractDataFactory::registerDataSerializerType(const QString& type, const QStringList& handled, medAbstractDataSerializerTypeCreator creator)
{
    bool success = false;

    if (!d->serializerTypes.contains(type))
    {
        d->serializerTypes.insert(type, medAbstractDataSerializerTypeInfo(handled, creator));
        success = true;
    }

    return success;
}

bool medAbstractDataFactory::registerDataDeserializerType(const QString& type, const QStringList& handled, medAbstractDataDeserializerTypeCreator creator)
{
    bool success = false;

    if (!d->deserializerTypes.contains(type))
    {
        d->deserializerTypes.insert(type, medAbstractDataDeserializerTypeInfo(handled, creator));
        success = true;
    }

    return success;
}

QStringList medAbstractDataFactory::creators() const
{
    return d->dataTypes.keys();
}

QStringList medAbstractDataFactory::readers() const
{
    QStringList result;

    if (d->readerPriorities.isEmpty())
    {
        result = d->readerTypes.keys();
    }
    else
    {
        foreach (QString reader, d->readerPriorities)
        {
            if (d->readerTypes.contains(reader))
            {
                result.append(reader);
            }
        }
    }

    return result;
}

QStringList medAbstractDataFactory::writers() const
{
    QStringList result;

    if (d->writerPriorities.isEmpty())
    {
        result = d->writerTypes.keys();
    }
    else
    {
        foreach (QString writer, d->writerPriorities)
        {
            if (d->writerTypes.contains(writer))
            {
                result.append(writer);
            }
        }
    }

    return result;
}

QStringList medAbstractDataFactory::converters() const
{
    QStringList result;

    if (d->converterPriorities.isEmpty())
    {
        result = d->converterTypes.keys();
    }
    else
    {
        foreach (QString converter, d->converterPriorities)
        {
            if (d->converterTypes.contains(converter))
            {
                result.append(converter);
            }
        }
    }

    return result;
}

QStringList medAbstractDataFactory::serializers() const
{
    return d->serializerTypes.keys();
}

QStringList medAbstractDataFactory::deserializers() const
{
    return d->deserializerTypes.keys();
}

void medAbstractDataFactory::setReaderPriorities(const QStringList& priorities)
{
    d->readerPriorities = priorities;
}

void medAbstractDataFactory::setWriterPriorities(const QStringList& priorities)
{
    d->writerPriorities = priorities;
}

void medAbstractDataFactory::setConverterPriorities(const QStringList& priorities)
{
    d->converterPriorities = priorities;
}

const QStringList& medAbstractDataFactory::readerPriorities() const
{
    return d->readerPriorities;
}

const QStringList& medAbstractDataFactory::writerPriorities() const
{
    return d->writerPriorities;
}

const QStringList& medAbstractDataFactory::converterPriorities() const
{
    return d->converterPriorities;
}

medAbstractData* medAbstractDataFactory::create(const QString& type)
{
    static int count = 0;
    medAbstractData* result = nullptr;

    if (d->dataTypes.contains(type))
    {
        result = d->dataTypes[type].creator();

        medAbstractDataReaderTypeHash::const_iterator readersIterator;
        medAbstractDataWriterTypeHash::const_iterator writersIterator;
        medAbstractDataConverterTypeHash::const_iterator convertersIterator;
        medAbstractDataSerializerTypeHash::const_iterator serializersIterator;
        medAbstractDataDeserializerTypeHash::const_iterator deserializersIterator;

        for (readersIterator = d->readerTypes.constBegin(); readersIterator != d->readerTypes.constEnd(); readersIterator++)
        {
            if (readersIterator.value().handled.contains(type))
            {
                result->addReader(readersIterator.key());
            }
        }

        for (writersIterator = d->writerTypes.constBegin(); writersIterator != d->writerTypes.constEnd(); writersIterator++)
        {
            if (writersIterator.value().handled.contains(type))
            {
                result->addWriter(writersIterator.key());
            }
        }

        for (convertersIterator = d->converterTypes.constBegin(); convertersIterator != d->converterTypes.constEnd(); convertersIterator++)
        {
            if (convertersIterator.value().fromTypes.contains(type))
            {
                result->addConverter(convertersIterator.key());
                result->enableConverter(convertersIterator.key());
            }
        }

        for (serializersIterator = d->serializerTypes.constBegin(); serializersIterator != d->serializerTypes.constEnd(); serializersIterator++)
        {
            if (serializersIterator.value().handled.contains(type))
            {
                result->addSerializer(serializersIterator.key());
                result->enableSerializer(serializersIterator.key());
            }
        }

        for (deserializersIterator = d->deserializerTypes.constBegin(); deserializersIterator != d->deserializerTypes.constEnd(); deserializersIterator++)
        {
            if (deserializersIterator.value().handled.contains(type))
            {
                result->addDeserializer(deserializersIterator.key());
                result->enableDeserializer(deserializersIterator.key());
            }
        }

        result->setObjectName(QString("%1%2").arg(result->metaObject()->className()).arg(count++));
    }

    return result;
}

dtkSmartPointer<medAbstractData> medAbstractDataFactory::createSmartPointer(const QString& type)
{
    return create(type);
}

dtkAbstractDataReader* medAbstractDataFactory::reader(const QString& type)
{
    dtkAbstractDataReader* result = nullptr;

    if (d->readerTypes.contains(type))
    {
        result = d->readerTypes[type].creator();
    }

    return result;
}

dtkSmartPointer<dtkAbstractDataReader> medAbstractDataFactory::readerSmartPointer(const QString& type)
{
    return reader(type);
}

dtkAbstractDataWriter* medAbstractDataFactory::writer(const QString& type)
{
    dtkAbstractDataWriter* result = nullptr;

    if (d->writerTypes.contains(type))
    {
        result = d->writerTypes[type].creator();
    }

    return result;
}

dtkSmartPointer<dtkAbstractDataWriter> medAbstractDataFactory::writerSmartPointer(const QString& type)
{
    return writer(type);
}

dtkAbstractDataConverter* medAbstractDataFactory::converter(const QString& type)
{
    dtkAbstractDataConverter* result = nullptr;

    if (d->converterTypes.contains(type))
    {
        result = d->converterTypes[type].creator();
    }

    return result;
}

dtkSmartPointer<dtkAbstractDataConverter> medAbstractDataFactory::converterSmartPointer(const QString& type)
{
    return converter(type);
}

dtkAbstractDataSerializer* medAbstractDataFactory::serializer(const QString& type)
{
    dtkAbstractDataSerializer* result = nullptr;

    if (d->serializerTypes.contains(type))
    {
        result = d->serializerTypes[type].creator();
    }

    return result;
}

dtkSmartPointer<dtkAbstractDataSerializer> medAbstractDataFactory::serializerSmartPointer(const QString& type)
{
    return serializer(type);
}

dtkAbstractDataDeserializer* medAbstractDataFactory::deserializer(const QString& type)
{
    dtkAbstractDataDeserializer* result = nullptr;

    if (d->deserializerTypes.contains(type))
    {
        result = d->deserializerTypes[type].creator();
    }

    return result;
}

dtkSmartPointer<dtkAbstractDataDeserializer> medAbstractDataFactory::deserializerSmartPointer(const QString& type)
{
    return deserializer(type);
}
