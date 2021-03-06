
#pragma once
#include"vtkCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageBlend.h"
#include <vtkImageViewer2.h>
#include "vtkCoordinate.h"
#include "MyAlgorithm3d.h"
#include "MyInteractionStyle3D.h"
#include "MyCanvas3D.h"


class MyCallback3D : public vtkCommand
{
public:
	static MyCallback3D *New();
	void SetImageReslice(vtkSmartPointer<vtkImageReslice>, vtkSmartPointer<vtkImageReslice>);
	void SetDiagram(MyCanvas3D*);
	void SetStyle(MyInteractionStyle3D*_style);
	void SetInteractor(vtkRenderWindowInteractor* interactor);

	virtual void Execute(vtkObject *, unsigned long event, void *);
	void SetRenderer(vtkSmartPointer<vtkRenderer>); //For Overlay
	void SetWindow(vtkSmartPointer<vtkRenderWindow>); //For Overlay
	InternalImageType* GetInternalImage();
	void Overlay();
	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
	VTKImageToImageType::Pointer GetFinalResult();

private:

	vtkSmartPointer<vtkImageReslice> ResliceDicom3D;
	vtkSmartPointer<vtkRenderWindowInteractor>Interactor;
	vtkSmartPointer<MyInteractionStyle3D> imageStyle;
	MyCanvas3D* IS_MyCanvas3D;

	typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;
	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalType;
	ImageType_2_InternalType::Pointer IS_InternalImage = ImageType_2_InternalType::New();

	MyAlgorithm3d* IS_Algorithm = new MyAlgorithm3d();

	//-----------------------Overlay-------------------
	typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;
	OutputImageType_2_ImageType::Pointer itkResult = OutputImageType_2_ImageType::New();

	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
	ConnectorType::Pointer vtkResult = ConnectorType::New();

	typedef itk::VTKImageToImageFilter<OutputImageType> VTKImageToImageType;
	VTKImageToImageType::Pointer itkFinalResult = VTKImageToImageType::New();
	
	vtkSmartPointer<vtkImageReslice> ResliceDicom2D;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> window;
};