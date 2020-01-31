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

#include "MySpeedFunction2D.h"
#include "InteractorStyle2D.h"
#include "Algorithm2D.h"
#include "Canvas2D.h"


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

typedef itk::ImageSource< InternalImageType> ReaderType;



class Callback2D : public vtkCommand
{
public:
	static Callback2D *New();
	void SetDiagram(Canvas2D* Diagram);

	void SetImageActor(vtkSmartPointer<vtkImageActor>);

	void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);
	vtkRenderWindowInteractor *GetInteractor();
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);

	virtual void Execute(vtkObject *, unsigned long event, void *);

	void SetStyle(InteractorStyle2D*);
	void SetSpeed(MySpeedType::Pointer);
	void SetReader(CastFilter::Pointer);
	Algorithm2D* GetAlgorithm() { return MySeg; }
	void set_image(ImageType*);
	void Overlay();

	//---------------Traditional---------------------
	//-----------------------------------------------
	/*Algorithm* GetAlgorithm2() { return MySeg2; }*/


private:

	VTK_CREATE(vtkRenderWindowInteractor, Interactor);
	VTK_CREATE(vtkRenderer, renderer);
	VTK_CREATE(vtkRenderWindow, window);
	VTK_CREATE(vtkImageActor, actor);
	ImageType* image;

	Canvas2D* diagram;
	InteractorStyle2D* style;
	Algorithm2D* MySeg = new Algorithm2D();
	MySpeedType::Pointer My_Function = MySpeedType::New();
	CastFilter::Pointer Reader;

	//---------------Traditional-------------------
	//---------------------------------------------
	Algorithm2D* MySeg2 = new Algorithm2D();
	MySpeedType::Pointer My_Function2 = MySpeedType::New();
	VTK_CREATE(vtkRenderWindowInteractor, renderWindowInteractor);
	VTK_CREATE(vtkImageViewer2, imageViewer);
	VTK_CREATE(vtkRenderWindowInteractor, renderWindowInteractor2);
	VTK_CREATE(vtkImageViewer2, imageViewer2);
};