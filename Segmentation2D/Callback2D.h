#pragma once
#include"vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageReslice.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDICOMImageReader.h"
#include"vtkSmartPointer.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include "vtkImageData.h"
#include "vtkCoordinate.h"
#include <itkImageSource.h>
#include "InteractorStyle2D.h"
#include "Algorithm2D.h"
#include "Canvas2D.h"
#include "SpeedFunction2D.h"
#include "ImageTypeDetails2D.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


class Callback2D : public vtkCommand
{
public:
	static Callback2D* New();
	void SetInternalImage(InternalImageType2D::Pointer);
	void SetDiagram(Canvas2D* Diagram);
	void SetStyle(InteractorStyle2D*);

	void SetImageActor(vtkSmartPointer<vtkImageActor>);

	void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);

	virtual void Execute(vtkObject*, unsigned long event, void*);


	typedef SpeedFunction2D< InternalImageType2D, InternalImageType2D > MySpeedFunction2DType;
	void SetSpeed(MySpeedFunction2DType::Pointer);

	Algorithm2D* GetAlgorithm() { return MySeg; }
	void Overlay();



private:

	InternalImageType2D::Pointer Reader;

	VTK_CREATE(vtkRenderWindowInteractor, Interactor);
	VTK_CREATE(vtkRenderer, renderer);
	VTK_CREATE(vtkRenderWindow, window);
	VTK_CREATE(vtkImageActor, actor);

	Canvas2D* diagram;
	InteractorStyle2D* style;
	Algorithm2D* MySeg = new Algorithm2D();
	MySpeedFunction2DType::Pointer My_Function;

};