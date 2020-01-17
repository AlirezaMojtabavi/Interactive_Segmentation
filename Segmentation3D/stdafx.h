// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL, vtkInteractionStyle)

#include <algorithm>
#include <iostream>
#include "vtkSmartPointer.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include <vtkDICOMImageReader.h>
#include <vtkImageMapper3D.h>
#include "vtkCamera.h"
#include "vtkRendererCollection.h"
#include "vtkContextActor.h"
#include <itkVTKImageToImageFilter.h>
#include "itkBinaryFillholeImageFilter.h"
#include <itkImageToVTKImageFilter.h>
#include <vtkImageSliceMapper.h>
#include "itkCastImageFilter.h"

const  unsigned int  Dimension = 3;
typedef  float  InternalPixelType;
typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

typedef itk::Image < unsigned short, Dimension > ImageType;

typedef unsigned short OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()



// TODO: reference additional headers your program requires here
