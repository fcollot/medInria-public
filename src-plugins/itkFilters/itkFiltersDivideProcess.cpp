/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkFiltersDivideProcess.h>

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkDivideImageFilter.h>
#include <itkImage.h>

#include <medUtilities.h>

class itkFiltersDivideProcessPrivate
{
public:
    double divideFactor;
};

const double itkFiltersDivideProcess::defaultDivideFactor = 2.0;

//-------------------------------------------------------------------------------------------

itkFiltersDivideProcess::itkFiltersDivideProcess(itkFiltersDivideProcess *parent) 
    : itkFiltersProcessBase(parent), d(new itkFiltersDivideProcessPrivate)
{  
    d->divideFactor = defaultDivideFactor;
}

itkFiltersDivideProcess::itkFiltersDivideProcess(const itkFiltersDivideProcess& other)
     : itkFiltersProcessBase(other), d(other.d)
{
}

//-------------------------------------------------------------------------------------------

itkFiltersDivideProcess::~itkFiltersDivideProcess( void )
{
    delete d;
}

//-------------------------------------------------------------------------------------------

bool itkFiltersDivideProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkDivideProcess", createitkFiltersDivideProcess);
}

//-------------------------------------------------------------------------------------------

QString itkFiltersDivideProcess::description() const
{
    return tr("ITK divide by constant filter");
}

//-------------------------------------------------------------------------------------------

void itkFiltersDivideProcess::setParameter(double data, int channel)
{
    if (channel != 0)
        return;
    
    d->divideFactor = data;
}

//-------------------------------------------------------------------------------------------

int itkFiltersDivideProcess::tryUpdate()
{
    int res = DTK_FAILURE;
    
    if ( getInputData() )
    {
        res = DISPATCH_ON_PIXEL_TYPE(&itkFiltersDivideProcess::updateProcess, this, getInputData());
    }

    return res;
}

template <class PixelType> int itkFiltersDivideProcess::updateProcess()
{
    typedef itk::Image< PixelType, 3 > ImageType;
    typedef itk::DivideImageFilter< ImageType, itk::Image<double, ImageType::ImageDimension>, ImageType >  DivideFilterType;
    typename DivideFilterType::Pointer divideFilter = DivideFilterType::New();

    divideFilter->SetInput ( dynamic_cast<ImageType *> ( ( itk::Object* ) ( getInputData()->data() ) ) );
    divideFilter->SetConstant ( d->divideFactor );

    itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
    callback->SetClientData ( ( void * ) this );
    callback->SetCallback ( itkFiltersProcessBase::eventCallback );
    divideFilter->AddObserver ( itk::ProgressEvent(), callback );

    divideFilter->Update();

    getOutputData()->setData ( divideFilter->GetOutput() );

    QString newSeriesDescription = "divide filter " + QString::number(d->divideFactor);
    medUtilities::setDerivedMetaData(getOutputData(), getInputData(), newSeriesDescription);

    return DTK_SUCCEED;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createitkFiltersDivideProcess ( void )
{
    return new itkFiltersDivideProcess;
}
