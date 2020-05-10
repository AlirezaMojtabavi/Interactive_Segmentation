
#pragma once
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkImageProperty.h"


class MyInteractionStyle3D : public vtkInteractorStyleImage
{

public:

	virtual void OnKeyPress();
	int GetFlag();
	virtual void WindowLevel();

private:

	int flag = 0;
};