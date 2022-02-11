#include "medPipelineFactory.h"

#include <stdexcept>

#include "medPythonPipelineInstance.h"

namespace med::pipeline
{

class PipelineFactoryPrivate
{
public:
    static QScopedPointer<PipelineFactory> instance;

    QHash<QString, std::function<PipelineDelegate* ()> > creators;
    QHash<QString, QString> titles;
};

QScopedPointer<PipelineFactory> PipelineFactoryPrivate::instance;

PipelineFactory* PipelineFactory::instance()
{
    if (PipelineFactoryPrivate::instance.isNull())
    {
        PipelineFactoryPrivate::instance.reset(new PipelineFactory);
    }

    return PipelineFactoryPrivate::instance.data();
}

PipelineFactory::PipelineFactory() :
    d(new PipelineFactoryPrivate())
{
}

PipelineFactory::~PipelineFactory()
{
    delete d;
}

void PipelineFactory::registerDelegate(python::Object pythonDelegate)
{
    if (!pythonDelegate.isSubType<PipelineDelegate>())
    {
        QString typeName = pythonDelegate.attribute("__name__").convert<QString>();
        QString message = QString("Type '%1' is not a pipeline delegate.").arg(typeName);
        throw python::TypeError(qUtf8Printable(message));
    }

    auto creator = [=]() -> PipelineDelegate*
    {
        return pythonDelegate().cast<PipelineDelegate>();
    };

    registerDelegate(pythonDelegate.typeName(), creator);
}

void PipelineFactory::registerPipeline(python::Object pythonPipeline)
{
    if (!pythonPipeline.isSubType<AbstractPipeline>())
    {
        QString typeName = pythonPipeline.attribute("__name__").convert<QString>();
        QString message = QString("Type '%1' is not a pipeline.").arg(typeName);
        throw python::TypeError(qUtf8Printable(message));
    }

    auto creator = [=]() -> AbstractPipeline*
    {
        return pythonPipeline().cast<AbstractPipeline>();
    };

    QString title = pythonPipeline.attribute("title").convert<QString>();
    registerPipeline(pythonPipeline.typeName(), title, creator);
}

PipelineDelegate* PipelineFactory::createDelegate(QString identifier)
{
    if (!d->creators.contains(identifier))
    {
        QString message = QString("PipelineFactory: no creator registered for %1.")
                          .arg(identifier);

        throw std::runtime_error(qUtf8Printable(message));
    }

    return d->creators[identifier]();
}

AbstractPipeline* PipelineFactory::createPipeline(QString identifier, QString sourceCode)
{
    AbstractPipeline* pipeline = nullptr;

    if (sourceCode.isNull())
    {
        pipeline = dynamic_cast<AbstractPipeline*>(createDelegate(identifier));

        if (!pipeline)
        {
            QString message = QString("PipelineFactory: delegate of type %1 is not a proper pipeline.")
                              .arg(identifier);

            throw std::runtime_error(qUtf8Printable(message));
        }
    }
    else
    {
        python::Object sourceObjects = python::runSourceCode(sourceCode);
        python::Object pythonPipeline = sourceObjects[identifier]();
        pipeline = pythonPipeline.cast<AbstractPipeline>();
    }

    return pipeline;
}

QStringList PipelineFactory::getRegisteredPipelines()
{
    return d->titles.keys();
}

QString PipelineFactory::getPipelineTitle(QString identifier)
{
    return d->titles[identifier];
}

void PipelineFactory::registerDelegate(QString identifier, std::function<PipelineDelegate* ()> creator)
{
    d->creators[identifier] = creator;
}

void PipelineFactory::registerPipeline(QString identifier, QString title, std::function<AbstractPipeline* ()> creator)
{
    d->creators[identifier] = creator;
    d->titles[identifier] = title;
}

void PipelineFactory::throwNoCreatorException(QString identifier)
{

}

} // namespace med::pipeline
