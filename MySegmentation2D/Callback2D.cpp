#include "stdafx.h"
#include "Callback2D.h"
#include "vtkImageBlend.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

Callback2D * Callback2D::New()
{
	return new Callback2D;
}

void Callback2D::SetReader(ImageType_2_InternalImageType::Pointer input_algorithm)
{
	IS2D_InternalImage = input_algorithm;
}

void Callback2D::SetDiagram(Canvas2D* _Diagram)
{
	diagram = _Diagram;
}

void Callback2D::SetStyle(InteractorStyle2D* _style)
{
	style = _style;
}

void Callback2D::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> _interactor)
{
	this->Interactor = _interactor;
}

void Callback2D::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void Callback2D::SetWindow(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

inline void Callback2D::Execute(vtkObject *caller, unsigned long event, void *)
{
	if (style->GetFlag() > 0)
	{
		double x = Interactor->GetEventPosition()[0];
		double y = Interactor->GetEventPosition()[1];

		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1], 0);
		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		double* position = Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint();
		diagram->SetLastposition(position[0], position[1]);

		//cout << "X: " << position[0] << "\t\t\tY: " << position[1] << "\n\n\n";
		Interactor->Render();

		if (style->GetFlag() == 1 || style->GetFlag() == 4 || style->GetFlag() == 5)
		{
			MySeg->SetInternalImage(IS2D_InternalImage);
			MySeg->SetCanvas(diagram);
			MySeg->FastMarching(4);
			MySeg->LevelSet(355, 650, 1.5, 0.05);

			if (style->GetFlag() == 4 || style->GetFlag() == 5 )
			{
				cout << "\t\t\tPlease Wait ..." << "\n\n\n";

				//MySeg->LevelSet(1.55, 0.05);
				MySeg->GetResult()->Update();
				this->Overlay();
			}
		}
	}
}

void Callback2D::SetOverlayImage(ImageType* _image)
{
	image = _image;		// overlay
}

void Callback2D::Overlay()
{
	//-------------------------MRI----------------------------------

	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(image);
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	ConnectorType::Pointer connector = ConnectorType::New();//convert itk image to vtk image
	connector->SetInput(rescaleFilter->GetOutput());
	connector->Update();

	//------------------------------------------------------------

	OutputImageType_2_ImageType::Pointer seg_caster = OutputImageType_2_ImageType::New();
	seg_caster->SetInput(MySeg->GetResult());
	seg_caster->Update();

	ConnectorType::Pointer thconnector = ConnectorType::New();
	thconnector->SetInput(seg_caster->GetOutput());
	thconnector->Update();

	//---------------------------viewer-----------------------------------

	auto blend = vtkSmartPointer<vtkImageBlend>::New();
	blend->AddInputData(thconnector->GetOutput());
	blend->AddInputData(connector->GetOutput());
	blend->SetOpacity(0.5, 0.5);
	blend->SetOpacity(1, .5);

	auto DataActor = vtkSmartPointer<vtkImageActor>::New();
	DataActor->GetMapper()->SetInputConnection(blend->GetOutputPort());
	renderer->AddActor(DataActor);
	window->AddRenderer(renderer);
	window->Render();

	static bool started = false;
	if (!started)
	{
		started = true;
		window->SetInteractor(Interactor);
	}
}
