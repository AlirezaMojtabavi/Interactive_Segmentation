

#include "stdafx.h"
#include "MyCallback3D.h"
#include "MyCanvas3D.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


MyCallback3D * MyCallback3D::New()
{
	return new MyCallback3D;
}

void MyCallback3D::SetImageReslice(vtkSmartPointer<vtkImageReslice> _ResliceDicom3D, vtkSmartPointer<vtkImageReslice> _ResliceDicom2D)
{
	ResliceDicom3D = _ResliceDicom3D;
	ResliceDicom2D = _ResliceDicom2D;

	vtkImageToImageFilter->SetInput(ResliceDicom3D->GetOutput());
	vtkImageToImageFilter->Update();

	IS_InternalImage->SetInput(vtkImageToImageFilter->GetOutput());
	IS_InternalImage->Update();
}

void MyCallback3D::SetStyle(MyInteractionStyle3D*_style)
{
	imageStyle = _style;
	IS_Algorithm->SetStyle(imageStyle);
}

void MyCallback3D::SetDiagram(MyCanvas3D* _canvas)
{
	IS_MyCanvas3D = _canvas;
}

void MyCallback3D::SetInteractor(vtkRenderWindowInteractor * interactor)
{
	this->Interactor = interactor;
}

void MyCallback3D::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void MyCallback3D::SetWindow(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

inline void MyCallback3D::Execute(vtkObject *caller, unsigned long event, void *)
{
	//const char * outputDirectory1 = "E:\\Interactive_Segmentation\\output3D";//Directory for write output

	if (event == vtkCommand::MouseWheelForwardEvent)
	{
		IS_MyCanvas3D->MouseWheelForward();
	}
	else if (event == vtkCommand::MouseWheelBackwardEvent)
	{
		IS_MyCanvas3D->MouseWheelBackward();
	}
	else
	{
		if (imageStyle->GetFlag() > 0)
		{
			//auto Interactor = (vtkRenderWindowInteractor*)(caller);
			double x = Interactor->GetEventPosition()[0];
			double y = Interactor->GetEventPosition()[1];
			int z = IS_MyCanvas3D->GetCurrentSlice();

			Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1], 0);
			Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
			double* position = Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint();

			IS_MyCanvas3D->SetLastposition(position[0], position[1], z);

			//cout << "X: " << position[0] << "\t\tY: " << position[1] << "\t\tZ: " << z << "\n\n\n";
			Interactor->Render();

			if (imageStyle->GetFlag() == 1 || imageStyle->GetFlag() == 4 || imageStyle->GetFlag() == 5)
			{
				IS_Algorithm->SetInternalImage(IS_InternalImage->GetOutput());
				IS_Algorithm->SetCanvas(IS_MyCanvas3D);
				IS_Algorithm->FastMarching(5);
				IS_Algorithm->LevelSet(500, 1500, 0, 0.05);

				if (imageStyle->GetFlag() == 4 || imageStyle->GetFlag() == 5)
				{
					cout << "\t\t\tPlease Wait ..." << "\n\n\n";

					//MySeg->LevelSet(1, 0.05);
					IS_Algorithm->GetThresholder()->Update();
					this->Overlay();
				}
			}
		}
	}
}

void MyCallback3D::Overlay()
{
	typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(vtkImageToImageFilter->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
	ConnectorType::Pointer mrconnector = ConnectorType::New();
	mrconnector->SetInput(rescaleFilter->GetOutput());
	mrconnector->Update();

	typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;
	OutputImageType_2_ImageType::Pointer OutputImage_2_Image = OutputImageType_2_ImageType::New();
	OutputImage_2_Image->SetInput(IS_Algorithm->GetThresholder());
	OutputImage_2_Image->Update();

	ConnectorType::Pointer thconnector = ConnectorType::New();
	thconnector->SetInput(OutputImage_2_Image->GetOutput());
	thconnector->Update();

	auto blend = vtkSmartPointer<vtkImageBlend>::New();
	blend->AddInputData(thconnector->GetOutput());
	blend->AddInputData(mrconnector->GetOutput());
	blend->SetOpacity(0.5, 0.5);
	blend->SetOpacity(1, .5);
	blend->Update();

	ResliceDicom2D->SetInputConnection(blend->GetOutputPort());

	auto DataActorOverlay = vtkSmartPointer<vtkImageActor>::New();
	DataActorOverlay->GetMapper()->SetInputConnection(ResliceDicom2D->GetOutputPort());
	renderer->AddActor(DataActorOverlay);
	window->AddRenderer(renderer);
	window->Render();
}