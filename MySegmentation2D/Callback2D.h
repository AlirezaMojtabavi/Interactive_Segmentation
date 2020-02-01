#pragma once
#include"vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageData.h"
#include "vtkCoordinate.h"
#include <itkImageSource.h>
#include <vtkImageViewer2.h>

#include "MySpeedFunction2D.h"
#include "InteractorStyle2D.h"
#include "Algorithm2D.h"
#include "Canvas2D.h"


class Callback2D : public vtkCommand
{
public:
	static Callback2D *New();
	void SetReader(ImageType_2_InternalImageType::Pointer input_algorithm);
	void set_image(ImageType*); //overlay
	void SetDiagram(Canvas2D* Diagram);
	void SetStyle(InteractorStyle2D*);

	void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);

	typedef MySpeedFunction2D< InternalImageType, InternalImageType > MySpeedFunction2DType;
	void SetSpeed(MySpeedFunction2DType::Pointer);
	Algorithm2D* GetAlgorithm() { return MySeg; }
	void Execute(vtkObject *, unsigned long event, void *);
	void Overlay();

private:

	ImageType_2_InternalImageType::Pointer IS2D_InternalImage;
	vtkSmartPointer<vtkRenderWindowInteractor> Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> window;

	Canvas2D* diagram;
	InteractorStyle2D* style;
	Algorithm2D* MySeg = new Algorithm2D();
	MySpeedFunction2DType::Pointer My_Function = MySpeedFunction2DType::New();

	//---------------Overlay-------------------
	//---------------------------------------------
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	ImageType* image;
};