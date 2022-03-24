%ignore medAbstractView::selectedRequest;
%ignore medAbstractLayeredView::layerAdded;
%ignore medAbstractLayeredView::layerRemoved;
%ignore medAbstractLayeredView::currentLayerChanged;

%{
#include "medAbstractLayeredView.h"
%}

%include "medAbstractView.h"
%include "medAbstractLayeredView.h"
