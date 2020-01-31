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
	void SetDiagram(Canvas2D* Diagram);

	void SetImageActor(vtkSmartPointer<vtkImageActor>);

	void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);

	virtual void Execute(vtkObject *, unsigned long event, void *);

	void SetStyle(InteractorStyle2D*);
	typedef MySpeedFunction2D< InternalImageType, InternalImageType > MySpeedFunction2DType;
	void SetSpeed(MySpeedFunction2DType::Pointer);
	void SetReader(ImageType_2_InternalImageType::Pointer);
	Algorithm2D* GetAlgorithm() { return MySeg; }
	void set_image(ImageType*);
	void Overlay();

private:

	VTK_CREATE(vtkRenderWindowInteractor, Interactor);
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> window;
	VTK_CREATE(vtkImageActor, actor);
	ImageType* image;

	Canvas2D* diagram;
	InteractorStyle2D* style;
	Algorithm2D* MySeg = new Algorithm2D();
	MySpeedFunction2DType::Pointer My_Function = MySpeedFunction2DType::New();
	ImageType_2_InternalImageType::Pointer Reader;

	//---------------Overlay-------------------
	//---------------------------------------------
	VTK_CREATE(vtkImageViewer2, imageViewer);

};