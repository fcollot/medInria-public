/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "itkNotOperator.h"

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkBinaryNotImageFilter.h>
#include <itkMinimumMaximumImageFilter.h>

#include <medAbstractDataFactory.h>
#include <medAttachedData.h>
#include <medUtilities.h>


bool itkNotOperator::registered()
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkNotOperator", createitkNotOperator);
}

QString itkNotOperator::description() const
{
    return "NOT";
}

void itkNotOperator::setInput (medAbstractData *data, int channel)
{
    if (channel == 0)
    {
        m_inputA = data;
    }
}

int itkNotOperator::update()
{
    int res = DTK_FAILURE;

    if (m_inputA)
    {
        res = DISPATCH_ON_PIXEL_TYPE(&itkNotOperator::run, this, m_inputA);
    }
    return res;
}


template <class PixelType> int itkNotOperator::run()
{
    typedef itk::Image<PixelType, 3> ImageType;

    if ( !m_inputA->data() )
    {
        return DTK_FAILURE;
    }
    typename ImageType::Pointer image = dynamic_cast<ImageType  *> ( ( itk::Object* ) ( m_inputA->data() )) ;

    typedef itk::MinimumMaximumImageFilter <ImageType> ImageCalculatorFilterType;
    typename ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New();
    imageCalculatorFilter->SetInput(image);
    imageCalculatorFilter->Update();

    typedef itk::BinaryNotImageFilter <ImageType> NotImageFilterType;
    typename NotImageFilterType::Pointer notFilter = NotImageFilterType::New();
    notFilter->SetInput(image);
    notFilter->SetBackgroundValue(imageCalculatorFilter->GetMinimum());
    notFilter->SetForegroundValue(imageCalculatorFilter->GetMaximum());
    notFilter->Update();

    QString identifier = m_inputA->identifier();
    m_output = medAbstractDataFactory::instance()->createSmartPointer ( identifier );

    m_output->setData(notFilter->GetOutput());

    if (!m_output)
    {
        return DTK_FAILURE;
    }

    medUtilities::setDerivedMetaData(m_output, m_inputA, "NOT");

    return DTK_SUCCEED;
}        

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createitkNotOperator()
{
    return new itkNotOperator;
}
