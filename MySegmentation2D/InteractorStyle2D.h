#pragma once
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkImageProperty.h"
#include "vtkRenderer.h"
#include "vtkImageData.h"
#include "vtkPointData.h"


class InteractorStyle2D : public vtkInteractorStyleImage
{

public:

	virtual void OnKeyPress();
	int GetFlag();
	virtual void WindowLevel();

private:

	int flag = 1;
};