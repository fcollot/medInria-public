%{
#include "medPipelineFactory.h"
%}

class med::pipeline::PipelineFactory
{
public:
    static PipelineFactory* instance();

    void registerDelegate(med::python::Object pythonDelegate);
    void registerPipeline(med::python::Object pythonPipeline);

private:
    PipelineFactory();
};

%forwardFunction(registerDeligate, PipelineFactory.instance())
%forwardFunction(registerPipeline, PipelineFactory.instance())
