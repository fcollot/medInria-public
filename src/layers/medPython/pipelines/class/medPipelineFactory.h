#pragma once

#include "medAbstractPipeline.h"
#include "medPipelineExport.h"

namespace med::pipeline
{

class PipelineFactoryPrivate;

class MEDPIPELINE_EXPORT PipelineFactory
{
public:
    static PipelineFactory* instance();

    ~PipelineFactory();

    template <typename TYPE>
    void registerDelegate();

    template <typename TYPE>
    void registerPipeline();

    void registerDelegate(python::Object pythonDelegate);
    void registerPipeline(python::Object pythonPipeline);

    PipelineDelegate* createDelegate(QString identifier);
    AbstractPipeline* createPipeline(QString identifier, QString sourceCode = QString());

    QStringList getRegisteredPipelines();

    QString getPipelineTitle(QString identifier);

private:
    PipelineFactory();

    template <typename TYPE>
    static PipelineDelegate* createDelegate(void* argument);

    template <typename TYPE>
    static AbstractPipeline* createPipeline(void* argument);

    PipelineFactoryPrivate* const d;

    void registerDelegate(QString identifier, std::function<PipelineDelegate*()> creator);
    void registerPipeline(QString identifier, QString title, std::function<AbstractPipeline*()> creator);

    void throwNoCreatorException(QString identifier);
};

template <typename TYPE>
void PipelineFactory::registerDelegate()
{
    auto creator = []() -> PipelineDelegate*
    {
        return new TYPE();
    };

    registerDelegate(TYPE::staticMetaObject.className(), creator);
}

template <typename TYPE>
void PipelineFactory::registerPipeline()
{
    auto creator = []() -> AbstractPipeline*
    {
        return new TYPE();
    };

    registerPipeline(TYPE::staticMetaObject.className(), TYPE::title, creator);
}

} // namespace med::pipeline

