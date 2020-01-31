#include "stdafx.h"
#include "Callback2D.h"
#include "Canvas2D.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

Callback2D * Callback2D::New()
{
	return new Callback2D;
}

void Callback2D::SetStyle(InteractorStyle2D*_style)
{
	style = _style;
}


void Callback2D::SetSpeed(MySpeedType::Pointer _Function)
{
	My_Function = _Function;
}

void Callback2D::SetReader(CastFilter::Pointer _Reader)
{
	Reader = _Reader;
}

void Callback2D::set_image(ImageType* _image)
{
	image = _image;		// overlay
}

void Callback2D::Overlay()
{

	//-------------------------MRI----------------------------------

	typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(image);
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	typedef itk::ImageToVTKImageFilter<ImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(rescaleFilter->GetOutput());
	connector->Update();

	//------------------------------------------------------------

	typedef itk::CastImageFilter<OutputImageType, ImageType> CastFilter3;
	CastFilter3::Pointer seg_caster = CastFilter3::New();
	seg_caster->SetInput(MySeg->Get_thresholder());
	seg_caster->Update();

	typedef itk::ImageToVTKImageFilter<ImageType>   ConnectorType;
	ConnectorType::Pointer thconnector = ConnectorType::New();
	thconnector->SetInput(seg_caster->GetOutput());
	thconnector->Update();

	//------------------------traditional-----------------------------

	//CastFilter3::Pointer thcaster2 = CastFilter3::New();
	//thcaster2->SetInput(MySeg2->Get_thresholder());
	//thcaster2->Update();

	//typedef itk::ImageToVTKImageFilter<ImageType>   ConnectorType;
	//ConnectorType::Pointer thconnector2 = ConnectorType::New();
	//thconnector2->SetInput(thcaster2->GetOutput());
	//thconnector2->Update();

	//---------------------------viewer-----------------------------------

	VTK_CREATE(vtkImageBlend, blend);
	blend->AddInputData(thconnector->GetOutput());
	blend->AddInputData(connector->GetOutput());
	blend->SetOpacity(0.5, 0.5);
	blend->SetOpacity(1, .5);

	//--------------------------teraditional---------------------------------

	//VTK_CREATE(vtkImageBlend, blend2);
	//blend2->AddInputData(thconnector2->GetOutput());
	//blend2->AddInputData(connector->GetOutput());
	//blend2->SetOpacity(0.5, 0.5);
	//blend2->SetOpacity(1, .5);

	//----------------------------------------------

	VTK_CREATE(vtkImageActor, DataActor);
	DataActor->GetMapper()->SetInputConnection(blend->GetOutputPort());
	renderer->AddActor(DataActor);
	window->AddRenderer(renderer);
	window->Render();

	//imageViewer2->SetInputConnection(blend2->GetOutputPort());
	//imageViewer2->Render();

	static bool started = false;
	if (!started)
	{
		started = true;
		window->SetInteractor(Interactor);
		//imageViewer2->SetupInteractor(renderWindowInteractor2);
		//renderWindowInteractor2->Start();
	}
}

void Callback2D::SetDiagram(Canvas2D* Diagram)
{
	diagram = Diagram;
}


inline void Callback2D::SetImageActor(vtkSmartPointer<vtkImageActor> image_actor)
{
	this->actor = image_actor;
}


void Callback2D::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	this->Interactor = interactor;
}

inline vtkRenderWindowInteractor * Callback2D::GetInteractor()
{
	return this->Interactor;
}

void Callback2D::set_renderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void Callback2D::set_window(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

inline void Callback2D::Execute(vtkObject *caller, unsigned long event, void *)
{

	std::string outputFilename1 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\my_curve.dcm"; // level set output
	//std::string outputFilename2 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\Traditional_curve.dcm"; // level set output

	 //std::string outputFilename1 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\my_curve.jpg"; // level set output
	 //std::string outputFilename2 = "C:\\Users\\Alireza\\Desktop\\Result_Write\\Traditional_curve.jpg"; // level set output

	vtkRenderWindowInteractor *interactor = this->GetInteractor();

	if (style->GetFlag() > 0)
	{
		auto interctor = (vtkRenderWindowInteractor*)(caller);
		double x = interctor->GetEventPosition()[0];
		double y = interctor->GetEventPosition()[1];

		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(interctor->GetEventPosition()[0], interctor->GetEventPosition()[1], 0);
		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		double* position = Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint();
		diagram->SetLastposition(position[0], position[1]);

		//cout << "X: " << position[0] << "\t\t\tY: " << position[1] << "\n\n\n";
		Interactor->Render();

		if (style->GetFlag() == 4 || style->GetFlag() == 1 || style->GetFlag() == 7 || style->GetFlag() == 8)
		{
			MySeg->set_reader(Reader);
			MySeg->Set_Function(My_Function);
			MySeg->set_Canvas(diagram);
			MySeg->FastMarching(4);
			MySeg->Level_Set(355, 650, 1.5, 0.05);

			//MySeg2->set_reader(Reader);
			//MySeg2->Set_Function(My_Function2);
			//MySeg2->set_Canvas(diagram);
			//MySeg2->FastMarching(4);
			//MySeg2->Level_Set(445, 700, 0, 0.05);

			if (style->GetFlag() == 4 || style->GetFlag() == 7 || style->GetFlag() == 8)
			{

				cout << "\t\t\tPlease Wait ..." << "\n\n\n";

				//MySeg->Level_Set(1.55, 0.05);
				MySeg->WriteImage(outputFilename1);

				//MySeg2->Level_Set( 0, 0.05);
				//MySeg2->WriteImage(outputFilename2);

				this->Overlay();

			}
		}
	}
}

