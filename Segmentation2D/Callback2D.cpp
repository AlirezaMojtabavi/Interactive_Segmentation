#include "stdafx.h"
#include "Callback2D.h"
#include "Canvas2D.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

Callback2D* Callback2D::New()
{
	return new Callback2D;
}

void Callback2D::SetInternalImage(InternalImageType2D::Pointer _Reader)
{
	Reader = _Reader;
}

void Callback2D::SetDiagram(Canvas2D* Diagram)
{
	diagram = Diagram;
}

void Callback2D::SetStyle(InteractorStyle2D* _style)
{
	style = _style;
}


void Callback2D::SetSpeed(MySpeedFunction2DType::Pointer _Function)
{
	My_Function = _Function;
}


void Callback2D::Overlay()
{

	//-------------------------MRI----------------------------------

	typedef itk::RescaleIntensityImageFilter< InternalImageType2D, InternalImageType2D > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(Reader);
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	typedef itk::ImageToVTKImageFilter<InternalImageType2D>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(rescaleFilter->GetOutput());
	connector->Update();

	//------------------------------------------------------------

	OutputImageType2D_2_InternalImageType2D::Pointer seg_caster = OutputImageType2D_2_InternalImageType2D::New();
	seg_caster->SetInput(MySeg->Get_thresholder());
	seg_caster->Update();

	typedef itk::ImageToVTKImageFilter<InternalImageType2D>   ConnectorType;
	ConnectorType::Pointer thconnector = ConnectorType::New();
	thconnector->SetInput(seg_caster->GetOutput());
	thconnector->Update();


	//---------------------------viewer-----------------------------------

	VTK_CREATE(vtkImageBlend, blend);
	blend->AddInputData(thconnector->GetOutput());
	blend->AddInputData(connector->GetOutput());
	blend->SetOpacity(0.5, 0.5);
	blend->SetOpacity(1, .5);

	VTK_CREATE(vtkImageActor, DataActor);
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




inline void Callback2D::SetImageActor(vtkSmartPointer<vtkImageActor> image_actor)
{
	this->actor = image_actor;
}


void Callback2D::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	this->Interactor = interactor;
}


void Callback2D::set_renderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void Callback2D::set_window(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

inline void Callback2D::Execute(vtkObject* caller, unsigned long event, void*)
{

	std::string outputFilename = "E:\\Interactive_Segmentation\\output2D\\my_final.dcm"; // level set output

	 //std::string outputFilename1 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\my_curve.jpg"; // level set output
	 //std::string outputFilename2 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\Traditional_curve.jpg"; // level set output

	if (style->GetFlag() > 0)
	{
		auto interctor = (vtkRenderWindowInteractor*)(caller);
		double x = Interactor->GetEventPosition()[0];
		double y = Interactor->GetEventPosition()[1];

		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1], 0);
		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		double* position = Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint();
		diagram->SetLastposition(position[0], position[1]);

		//cout << "X: " << position[0] << "\t\t\tY: " << position[1] << "\n\n\n";
		Interactor->Render();

		if (style->GetFlag() == 4 || style->GetFlag() == 1 || style->GetFlag() == 5)
		{
			MySeg->set_reader(Reader);
			MySeg->Set_Function(My_Function);
			MySeg->set_Canvas(diagram);
			MySeg->FastMarching(4);
			MySeg->Level_Set(355, 650, 1.5, 0.05);

			if (style->GetFlag() == 4 || style->GetFlag() == 5)
			{

				cout << "\t\t\tPlease Wait ..." << "\n\n\n";

				//MySeg->Level_Set(1.55, 0.05);
				MySeg->Get_thresholder()->Update();
				MySeg->WriteImage(outputFilename);
				this->Overlay();

			}
		}
	}
}

