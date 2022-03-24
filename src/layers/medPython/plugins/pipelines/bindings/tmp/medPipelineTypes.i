%{
  #include "medPipelineTypes.h"
%}

%include "medPipelineTypes.h"

%constant char* SURFACE_RENDERING = med::pipeline::RenderingParameters::SURFACE;
%constant char* WIREFRAME_RENDERING = med::pipeline::RenderingParameters::WIREFRAME;
%constant char* POINTS_RENDERING = med::pipeline::RenderingParameters::POINTS;
