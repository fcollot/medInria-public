import medInria as med

_pipelines = {}


def registerPipeline(identifier, pipeline):
    _pipelines[identifier] = pipeline
