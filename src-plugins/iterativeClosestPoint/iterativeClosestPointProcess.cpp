/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "iterativeClosestPointProcess.h"

#include <dtkCore/dtkAbstractProcessFactory.h>
#include <dtkCore/dtkSmartPointer.h>

#include <medAbstractDataFactory.h>
#include <medUtilities.h>

#include <vtkICPFilter.h>
#include <vtkMetaDataSet.h>
#include <vtkMetaSurfaceMesh.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// iterativeClosestPointProcessPrivate
// /////////////////////////////////////////////////////////////////

class iterativeClosestPointProcessPrivate
{
public:
    dtkSmartPointer <medAbstractData> inputSource;
    dtkSmartPointer <medAbstractData> inputTarget;
    dtkSmartPointer <medAbstractData> output;
    
    int bStartByMatchingCentroids;
    int bTransformation;
    int bCheckMeanDistance;

    double ScaleFactor;
    int MaxNumIterations;
    int MaxNumLandmarks;
    double MaxMeanDistance;
};

// /////////////////////////////////////////////////////////////////
// iterativeClosestPointProcess
// /////////////////////////////////////////////////////////////////

iterativeClosestPointProcess::iterativeClosestPointProcess() : medAbstractProcess(), d(new iterativeClosestPointProcessPrivate)
{
    d->inputSource = NULL;
    d->inputTarget = NULL;
    d->output = NULL;

    d->bStartByMatchingCentroids=1;
    d->bTransformation=0;
    d->bCheckMeanDistance=0;

    d->ScaleFactor=1;
    d->MaxNumIterations=100;
    d->MaxNumLandmarks=1000;
    d->MaxMeanDistance=1;
}

iterativeClosestPointProcess::~iterativeClosestPointProcess()
{
    delete d;
    d = NULL;
}

bool iterativeClosestPointProcess::registered()
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("iterativeClosestPointProcess", createiterativeClosestPointProcess);
}

QString iterativeClosestPointProcess::description() const
{
    return "iterativeClosestPointProcess";
}

void iterativeClosestPointProcess::setInputData(medAbstractData* data, int channel, int frame)
{
    Q_UNUSED(frame);

    if ( !data  || data->identifier()!="vtkDataMesh")
        return;
    
    if (channel==0)
    {
        d->inputSource = data;
    }
    
    if (channel==1)
    {
        d->inputTarget = data;
    }
}    

void iterativeClosestPointProcess::setDoubleParameter(double data, int channel, int frame)
{
    Q_UNUSED(frame);

    switch (channel)
    {
    case 3:
        d->MaxMeanDistance = data;
        break;
    case 6:
        d->ScaleFactor = data;
        break;
    }
}

void iterativeClosestPointProcess::setIntParameter(int data, int channel, int frame)
{
    Q_UNUSED(frame);

    switch (channel)
    {
    case 0:
        d->bStartByMatchingCentroids = data;
        break;
    case 1:
        d->bTransformation = data;
        break;
    case 2:
        d->bCheckMeanDistance = data;
        break;
    case 4:
        d->MaxNumIterations = data;
        break;
    case 5:
        d->MaxNumLandmarks = data;
        break;
    }
}

int iterativeClosestPointProcess::internalUpdate()
{
    if ( !d->inputSource || !d->inputTarget )
        return DTK_FAILURE;

    vtkSmartPointer<vtkICPFilter> ICPFilter = vtkICPFilter::New();
    
    vtkMetaDataSet * source_dataset = static_cast<vtkMetaDataSet*>(d->inputSource->data());
    vtkMetaDataSet * target_dataset = static_cast<vtkMetaDataSet*>(d->inputTarget->data());
    
    ICPFilter->SetSource(static_cast<vtkPolyData*>(source_dataset->GetDataSet()));
    ICPFilter->SetTarget(static_cast<vtkPolyData*>(target_dataset->GetDataSet()));

    ICPFilter->SetbStartByMatchingCentroids(d->bStartByMatchingCentroids);
    ICPFilter->SetbTransformation(d->bTransformation);
    ICPFilter->SetbCheckMeanDistance(d->bCheckMeanDistance);
    ICPFilter->SetScaleFactor(d->ScaleFactor);
    ICPFilter->SetMaxNumIterations(d->MaxNumIterations);
    ICPFilter->SetMaxNumLandmarks(d->MaxNumLandmarks);
    ICPFilter->SetMaxMeanDistance(d->MaxMeanDistance);

    ICPFilter->Update();

    vtkPolyData * output_polyData = ICPFilter->GetOutput();

    vtkMetaSurfaceMesh * output_mesh = vtkMetaSurfaceMesh::New();
    output_mesh->SetDataSet(output_polyData);

    d->output = medAbstractDataFactory::instance()->createSmartPointer ( "vtkDataMesh" );
    d->output->setData(output_mesh);
    medUtilities::setDerivedMetaData(d->output, d->inputSource, "ICP");

    return DTK_SUCCEED;
}

medAbstractData* iterativeClosestPointProcess::getOutputData(int channel, int frame) const
{
    Q_UNUSED(channel);
    Q_UNUSED(frame);

    return d->output;
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createiterativeClosestPointProcess()
{
    return new iterativeClosestPointProcess;
}
