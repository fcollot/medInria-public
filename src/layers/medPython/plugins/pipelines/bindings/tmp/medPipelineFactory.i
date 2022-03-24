%{
#include "medPipelineFactory.h"
#include "medPythonPipeline.h"
%}

%include "medPipelineFactory.h"

%inline
%{
    namespace
    {

    medAbstractPipeline* createPythonPipeline(void* argument)
    {
        PyObject* pipeline = PyObject_CallFunction(static_cast<PyObject*>(argument), nullptr);

        if (pipeline != nullptr)
        {
            void* voidPointer;
            swig_type_info* typeInfo = SWIG_TypeQuery("medPythonPipeline*");
            const int result = SWIG_ConvertPtr(pipeline, &voidPointer, typeInfo, 0);

            if (SWIG_IsOK(result))
            {
                return reinterpret_cast<medPythonPipeline*>(voidPointer);
            }
        }

        throw std::runtime_error("Cannot instantiate a python pipeline");
    }

    }
%}

%constant medAbstractPipeline* createPythonPipeline(void* argument);

%extend medPipelineFactory
{
    void registerPipeline(QString identifier, medPipelineFactory::pipelineCreatorFunction creator, PyObject* argument)
    {
        $self->registerPipeline(identifier, creator, static_cast<void*>(argument));
    }

    %pythoncode
    {
        def registerPythonPipeline(self, klass):
            assert issubclass(klass, Pipeline), "not a python class"
            self.registerPipeline(klass.__name__, createPythonPipeline, klass)
    }
}
