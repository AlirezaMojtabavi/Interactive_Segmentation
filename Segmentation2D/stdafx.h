#pragma once

#pragma once
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
