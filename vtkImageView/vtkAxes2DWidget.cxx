/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkViewImage2D.h 1218 2009-08-04 11:29:59Z filus $
Language:  C++
Author:    $Author: filus $
Date:      $Date: 2009-08-04 13:29:59 +0200 (mar, 04 aoû 2009) $
Version:   $Revision: 1218 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vtkAxes2DWidget.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkWidgetEventTranslator.h"
#include "vtkWidgetCallbackMapper.h"
#include "vtkEvent.h"
#include "vtkWidgetEvent.h"
#include "vtkCursor2D.h"
#include "vtkProperty2D.h"
#include <vtkCommand.h>
#include <vtkImageView2D.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtkPropCollection.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlane.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkRenderer.h>

vtkCxxRevisionMacro(vtkAxes2DWidget, "$Revision: 1.9 $");
vtkStandardNewMacro(vtkAxes2DWidget);

//----------------------------------------------------------------------------------
vtkAxes2DWidget::vtkAxes2DWidget()
{
  this->ImageView = 0;
  this->RenderWindow = 0;

  this->PlaneXmin = vtkPlane::New();
  this->PlaneXmax = vtkPlane::New();
  this->PlaneYmin = vtkPlane::New();
  this->PlaneYmax = vtkPlane::New();

  this->Source = vtkPolyData::New();
  this->Points = vtkPoints::New();
  this->Mapper = vtkPolyDataMapper::New();
  this->Actor  = vtkActor::New();
  this->Renderer = vtkRenderer::New();
  

  this->Source->SetPoints (this->Points);
  this->Mapper->SetInputConnection (this->Source->GetProducerPort());
  this->Actor->SetMapper (this->Mapper);
  this->Renderer->AddViewProp (this->Actor);

  this->Renderer->SetLayer (1);
  this->Renderer->InteractiveOff();
  this->Renderer->SetViewport (0.0, 0.0, 1.0, 1.0);
 
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  this->Points->InsertNextPoint (0,0,0);
  

  this->Source->Allocate();  
  vtkIdType line[2];
  line[0] = 0; line[1] = 1;  
  this->Source->InsertNextCell (VTK_LINE, 2, line);
  line[0] = 2; line[1] = 3;  
  this->Source->InsertNextCell (VTK_LINE, 2, line);
  line[0] = 4; line[1] = 5;  
  this->Source->InsertNextCell (VTK_LINE, 2, line);
  line[0] = 6; line[1] = 7;  
  this->Source->InsertNextCell (VTK_LINE, 2, line);
  
  this->Command = vtkAxes2DWidgetCommand::New();
  this->Command->SetWidget (this);

  this->ColorArray = vtkUnsignedCharArray::New();
  this->ColorArray->SetName ("Colors");
  this->ColorArray->SetNumberOfComponents (3);

  unsigned char red[3]   = {255,0,0};
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);
  this->ColorArray->InsertNextTupleValue (red);

  this->Source->GetPointData()->SetScalars (this->ColorArray);
  
  this->Enabled = 0;
  this->Radius = 2;
}

//----------------------------------------------------------------------------------
vtkAxes2DWidget::~vtkAxes2DWidget()
{
  this->PlaneXmin->Delete();
  this->PlaneXmax->Delete();
  this->PlaneYmin->Delete();
  this->PlaneYmax->Delete();
  this->Source->Delete();
  this->Points->Delete();
  this->Mapper->Delete();
  this->Actor->Delete();  
  this->Renderer->Delete();
  this->Command->Delete();
  this->ColorArray->Delete();
  if (this->RenderWindow)
    this->RenderWindow->Delete();
}

//----------------------------------------------------------------------
void vtkAxes2DWidget::SetImageView(vtkImageView2D* arg)
{
  if (this->ImageView && arg != this->ImageView)
  {
    this->ImageView->RemoveObserver (this->Command);
  }

  this->ImageView = arg;
  
  if (this->ImageView)
  {
    this->ComputePlanes();
  }
}

//----------------------------------------------------------------------
void vtkAxes2DWidget::SetRenderWindow(vtkRenderWindow *arg)
{
    if (this->RenderWindow && arg!=this->RenderWindow) {
        this->RenderWindow->RemoveRenderer(this->Renderer);
        this->Renderer->SetRenderWindow(NULL);
    }

    vtkSetObjectBodyMacro (RenderWindow, vtkRenderWindow, arg);
}

//----------------------------------------------------------------------
void vtkAxes2DWidget::SetEnabled(int enabling)
{

  this->Enabled = enabling;

  if( !this->ImageView ||
      (this->ImageView && !this->RenderWindow) )
  {
    vtkErrorMacro ( << "Please set Render Window before enabling the widget");
    return;
  }

  if (this->RenderWindow->GetNeverRendered())
    return;
  
  if (this->ImageView)
  {
    if (enabling)
    { 
      if (!this->ImageView->HasObserver (vtkImageView2D::CurrentPointChangedEvent, this->Command))
      {  
        this->ImageView->AddObserver (vtkImageView2D::CurrentPointChangedEvent, this->Command, 20.0);
        this->ImageView->AddObserver (vtkImageView2D::OrientationChangedEvent, this->Command, 20.0);
      }
      
      if (this->RenderWindow->GetNumberOfLayers() < 2)
        this->RenderWindow->SetNumberOfLayers( 2 );

      this->RenderWindow->AddRenderer (this->Renderer);

      this->Renderer->SetActiveCamera (this->ImageView->GetRenderer()->GetActiveCamera());

      this->ComputePlanes();
    }
    else
    {
      this->ImageView->RemoveObserver (this->Command);
      if( this->RenderWindow )
        this->RenderWindow->RemoveRenderer (this->Renderer);
    }
  }    
}

//----------------------------------------------------------------------
void vtkAxes2DWidget::ComputePlanes(void)
{
  if (!this->ImageView || !this->ImageView->GetRenderer() || !this->ImageView->GetImageActor())
    return;

  unsigned char red[3]   = {0,0,0};
  unsigned char green[3] = {0,0,0};
  unsigned char blue[3]  = {0,0,0};
  red[0] = 255; green[1] = 255; blue[2] = 255;
  
  unsigned char* colorX, *colorY;
  switch (this->ImageView->GetViewOrientation())
  {
      case vtkImageView2D::VIEW_ORIENTATION_SAGITTAL:
	colorX = green;
	colorY = blue;
	break;
      case vtkImageView2D::VIEW_ORIENTATION_CORONAL:
	colorX = red;
	colorY = blue;
	break;
      case vtkImageView2D::VIEW_ORIENTATION_AXIAL:
      default:
	colorX = red;
	colorY = green;
	break;
  }
  
  this->ColorArray->SetTupleValue (0, colorX);
  this->ColorArray->SetTupleValue (1, colorX);
  this->ColorArray->SetTupleValue (2, colorX);
  this->ColorArray->SetTupleValue (3, colorX);
  this->ColorArray->SetTupleValue (4, colorY);
  this->ColorArray->SetTupleValue (5, colorY);
  this->ColorArray->SetTupleValue (6, colorY);
  this->ColorArray->SetTupleValue (7, colorY);
  
  vtkCamera* cam = this->ImageView->GetRenderer()->GetActiveCamera();
  double* normal = cam->GetViewPlaneNormal();
  double* axisy = cam->GetViewUp();
  double axisx[3];
  vtkMath::Cross (axisy, normal, axisx);

  this->PlaneXmin->SetNormal (axisx);
  this->PlaneXmax->SetNormal (axisx);
  this->PlaneYmin->SetNormal (axisy);
  this->PlaneYmax->SetNormal (axisy);

  int* ext = this->ImageView->GetImageActor()->GetDisplayExtent();
  
  int imageposition[3];
  
  imageposition[0] = ext[0]; imageposition[1] = ext[2]; imageposition[2] = ext[4];
  double p1[3];
  this->ImageView->GetWorldCoordinatesFromImageCoordinates (imageposition, p1);
  imageposition[0] = ext[1]; imageposition[1] = ext[3]; imageposition[2] = ext[5];
  double p2[3];
  this->ImageView->GetWorldCoordinatesFromImageCoordinates (imageposition, p2);

  double d1[3], d2[3];
  this->ImageView->GetRenderer()->SetWorldPoint (p1[0], p1[1], p1[2], 0);
  this->ImageView->GetRenderer()->WorldToDisplay();
  this->ImageView->GetRenderer()->GetDisplayPoint (d1);
  this->ImageView->GetRenderer()->SetWorldPoint (p2[0], p2[1], p2[2], 0);
  this->ImageView->GetRenderer()->WorldToDisplay();
  this->ImageView->GetRenderer()->GetDisplayPoint (d2);

  this->PlaneXmin->SetOrigin (d1[0] < d2[0] ? p1 : p2);
  this->PlaneXmax->SetOrigin (d1[0] < d2[0] ? p2 : p1);
  this->PlaneYmin->SetOrigin (d1[1] < d2[1] ? p1 : p2);
  this->PlaneYmax->SetOrigin (d1[1] < d2[1] ? p2 : p1);
}

//----------------------------------------------------------------------
void vtkAxes2DWidget::ComputeLyingPoints(double* pos)
{
  //this->ComputePlanes(); // image may have changed, so we need to re-compute planes
  
  double p0[3], p1[3], p2[3], p3[3], r0[3], r1[3], r2[3], r3[3];

  for (unsigned int i=0; i<3; i++)
  {
    r0[i] = pos[i] - this->Radius * this->PlaneXmin->GetNormal()[i];
    r1[i] = pos[i] + this->Radius * this->PlaneXmin->GetNormal()[i];
    r2[i] = pos[i] - this->Radius * this->PlaneYmin->GetNormal()[i];
    r3[i] = pos[i] + this->Radius * this->PlaneYmin->GetNormal()[i];
  }
  
  vtkPlane::ProjectPoint (pos, this->PlaneXmin->GetOrigin(), this->PlaneXmin->GetNormal(), p0);
  vtkPlane::ProjectPoint (pos, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), p1);
  vtkPlane::ProjectPoint (pos, this->PlaneYmin->GetOrigin(), this->PlaneYmin->GetNormal(), p2);
  vtkPlane::ProjectPoint (pos, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), p3);
  
  if (this->PlaneYmax->EvaluateFunction (pos) > 0) 	 
  {
    vtkPlane::ProjectPoint (p0, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), p0);
    vtkPlane::ProjectPoint (p1, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), p1);
    vtkPlane::ProjectPoint (r0, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), r0);
    vtkPlane::ProjectPoint (r1, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), r1);
  } 	 
  if (this->PlaneYmin->EvaluateFunction (pos) < 0) 	 
  {
    vtkPlane::ProjectPoint (p0, this->PlaneYmin->GetOrigin(), this->PlaneYmin->GetNormal(), p0);
    vtkPlane::ProjectPoint (p1, this->PlaneYmin->GetOrigin(), this->PlaneYmin->GetNormal(), p1);
    vtkPlane::ProjectPoint (r0, this->PlaneYmin->GetOrigin(), this->PlaneYmin->GetNormal(), r0);
    vtkPlane::ProjectPoint (r1, this->PlaneYmin->GetOrigin(), this->PlaneYmin->GetNormal(), r1); 	 
  }
  if (this->PlaneXmax->EvaluateFunction (pos) > 0) 	 
  {
    vtkPlane::ProjectPoint (p2, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), p2); 
    vtkPlane::ProjectPoint (p3, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), p3);
    vtkPlane::ProjectPoint (r2, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), r2);
    vtkPlane::ProjectPoint (r3, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), r3); 	 
  } 	 
  if (this->PlaneXmin->EvaluateFunction (pos) < 0) 	 
  {
    vtkPlane::ProjectPoint (p2, this->PlaneXmin->GetOrigin(), this->PlaneXmin->GetNormal(), p2);
    vtkPlane::ProjectPoint (p3, this->PlaneXmin->GetOrigin(), this->PlaneXmin->GetNormal(), p3);
    vtkPlane::ProjectPoint (r2, this->PlaneXmin->GetOrigin(), this->PlaneXmin->GetNormal(), r2);
    vtkPlane::ProjectPoint (r3, this->PlaneXmin->GetOrigin(), this->PlaneXmin->GetNormal(), r3); 	 
  }

  if (this->PlaneXmax->EvaluateFunction (r0) > 0) 	 
    vtkPlane::ProjectPoint (r0, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), r0); 	 
  if (this->PlaneXmax->EvaluateFunction (r1) > 0) 	 
    vtkPlane::ProjectPoint (r1, this->PlaneXmax->GetOrigin(), this->PlaneXmax->GetNormal(), r1); 	 
  if (this->PlaneXmin->EvaluateFunction (r0) < 0) 	 
    vtkPlane::ProjectPoint (r0, this->PlaneXmin->GetOrigin(), this->PlaneXmax->GetNormal(), r0); 	 
  if (this->PlaneXmin->EvaluateFunction (r1) < 0) 	 
    vtkPlane::ProjectPoint (r1, this->PlaneXmin->GetOrigin(), this->PlaneXmax->GetNormal(), r1); 	 
  
  if (this->PlaneYmax->EvaluateFunction (r2) > 0) 	 
    vtkPlane::ProjectPoint (r2, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), r2); 	 
  if (this->PlaneYmax->EvaluateFunction (r3) > 0) 	 
    vtkPlane::ProjectPoint (r3, this->PlaneYmax->GetOrigin(), this->PlaneYmax->GetNormal(), r3); 	 
  if (this->PlaneYmin->EvaluateFunction (r2) < 0) 	 
    vtkPlane::ProjectPoint (r2, this->PlaneYmin->GetOrigin(), this->PlaneYmax->GetNormal(), r2); 	 
  if (this->PlaneYmin->EvaluateFunction (r3) < 0) 	 
    vtkPlane::ProjectPoint (r3, this->PlaneYmin->GetOrigin(), this->PlaneYmax->GetNormal(), r3); 	 

  this->Points->SetPoint (0, p0);
  this->Points->SetPoint (1, r0);
  this->Points->SetPoint (2, p1);
  this->Points->SetPoint (3, r1);
  this->Points->SetPoint (4, p2);
  this->Points->SetPoint (5, r2);
  this->Points->SetPoint (6, p3);
  this->Points->SetPoint (7, r3);
  
  this->Source->Modified();
}

//----------------------------------------------------------------------------------
void vtkAxes2DWidget::PrintSelf(ostream& os, vtkIndent indent)
{
  //Superclass typedef defined in vtkTypeMacro() found in vtkSetGet.h
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------------
void vtkAxes2DWidgetCommand::Execute(vtkObject *caller,
				     unsigned long event,
				     void *callData)
{
  if (event == vtkImageView2D::CurrentPointChangedEvent)
  {
    
    if (!this->Widget)
      return;
    
    vtkImageView *view = vtkImageView::SafeDownCast(caller);
    if (!view)
      return;
    
    double* pos = view->GetCurrentPoint();
    this->Widget->ComputeLyingPoints (pos);
  }
  if (event == vtkImageView2D::OrientationChangedEvent)
  {
    
    if (!this->Widget)
      return;
    vtkImageView *view = vtkImageView::SafeDownCast(caller);
    if (!view)
      return;
    
    double* pos = view->GetCurrentPoint();
    this->Widget->SetEnabled(this->Widget->GetEnabled());
    this->Widget->ComputePlanes();
    this->Widget->ComputeLyingPoints (pos);
  }
}
