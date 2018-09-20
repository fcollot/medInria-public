/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <itkFiltersProcessBase.h>

class medAbstractData;

class ITKFILTERSPLUGIN_EXPORT itkFiltersMedianProcess : public itkFiltersProcessBase
{
    Q_OBJECT
    
public:
    itkFiltersMedianProcess(itkFiltersMedianProcess * parent = 0);

    static bool registered ( void );

    virtual QString description(void) const;
    
public slots:

    int tryUpdate();

protected:
    template <class ImageType> int updateProcess(medAbstractData* inputData);
};

dtkAbstractProcess * createitkFiltersMedianProcess(void);



