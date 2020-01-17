#pragma once
#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include <vtkImageSliceMapper.h>
#include <itkCastImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include "MyCanvas3D.h"


class Reslicer
{
public:
	
	Reslicer(vtkSmartPointer<vtkDICOMImageReader> _Dicom) :IS_DicomInput(_Dicom) {}
	vtkSmartPointer < vtkDICOMImageReader> PrepareInputImage(vtkSmartPointer < vtkDICOMImageReader> _input);
	vtkSmartPointer< vtkMatrix4x4> GetAxes();
	void Reslicer::SetReslice3D(vtkSmartPointer<vtkImageReslice> _reslicer3d);
	void Reslicer::SetReslice2D(vtkSmartPointer<vtkImageReslice> _reslicer2d);
	void SetCanvas(MyCanvas3D* _Canvas);


private:
	int extent[6];
	double spacing[3];
	double origin[3];
	double center[3];
	const double axialElements[16] = {
							1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1 };



	vtkSmartPointer<vtkDICOMImageReader> IS_DicomInput;
	vtkSmartPointer< vtkImageReslice> Reslise3D;
	vtkSmartPointer< vtkImageReslice> Reslise2D;
	
	MyCanvas3D* IS_MyCanvas3D;
	VTK_CREATE(vtkMatrix4x4, ResliceAxes);
};
