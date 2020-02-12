#include "stdafx.h"
#include "Reslicer.h"


vtkSmartPointer<vtkDICOMImageReader> Reslicer::PrepareInputImage(vtkSmartPointer<vtkDICOMImageReader> _input)
{
	_input->GetOutput()->GetExtent(extent);
	_input->GetOutput()->GetSpacing(spacing);
	_input->GetOutput()->GetOrigin(origin);
	_input->GetDataExtent();

	center[0] = origin[0];// +spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1];// +spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2];// +spacing[2] * 0.5 * (extent[4] + extent[5]);

	return _input;
}

 vtkSmartPointer<vtkMatrix4x4> Reslicer::GetAxes()
{
	ResliceAxes->DeepCopy(axialElements);
	ResliceAxes->SetElement(0, 3, center[0]);
	ResliceAxes->SetElement(1, 3, center[1]);
	ResliceAxes->SetElement(2, 3, center[2]);
	ResliceAxes->Modified();
	return ResliceAxes;
}

void Reslicer::SetReslice3D(vtkSmartPointer<vtkImageReslice> _reslicer3d)
{
	_reslicer3d->SetInputConnection(IS_DicomInput->GetOutputPort());
	_reslicer3d->SetOutputDimensionality(3);
	_reslicer3d->SetResliceAxes(this->GetAxes());
	_reslicer3d->SetInterpolationModeToLinear();
	_reslicer3d->Update();
}

 void Reslicer::SetReslice2D(vtkSmartPointer<vtkImageReslice> _reslice2d)
{
	 _reslice2d->SetInputData(IS_MyCanvas3D->GetImage());
	 _reslice2d->SetOutputDimensionality(2);
	 _reslice2d->SetResliceAxes(this->GetAxes());
	 _reslice2d->SetInterpolationModeToLinear();
	 _reslice2d->Update();
}

void Reslicer::SetCanvas(MyCanvas3D* _Canvas)
{
	IS_MyCanvas3D = _Canvas;
}

double* Reslicer::GetSpacing()
{
	return spacing;
}
