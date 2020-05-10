
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

InternalImageType* MyCallback3D::GetInternalImage()
{
	return IS_InternalImage->GetOutput();
}

inline void MyCallback3D::Execute(vtkObject *caller, unsigned long event, void *)
{
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

			cout << "X: " << position[0] << "\t\tY: " << position[1] << "\t\tZ: " << z << "\n\n\n";
			Interactor->Render();

			if (imageStyle->GetFlag() == 1 || imageStyle->GetFlag() == 4 || imageStyle->GetFlag() == 5)
			{
				IS_Algorithm->SetInternalImage(IS_InternalImage->GetOutput());
				IS_Algorithm->SetCanvas(IS_MyCanvas3D);
				IS_Algorithm->FastMarching(5);
				IS_MyCanvas3D->GetROI();
				IS_Algorithm->LevelSet(185, 410, 0.5, 0.05);

				if (imageStyle->GetFlag() == 4 || imageStyle->GetFlag() == 5)
				{
					cout << "\t\t\tPlease Wait ..." << "\n\n\n";

					//MySeg->LevelSet(1, 0.05);
					IS_Algorithm->GetThresholder()->Update();
					this->Overlay();
				}
			}
			if (imageStyle->GetFlag() == 2 || imageStyle->GetFlag() == 3)
			{
				this->Overlay();
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

	if ((imageStyle->GetFlag() != 2) || (imageStyle->GetFlag() != 3))
	{
		itkResult->SetInput(IS_Algorithm->GetThresholder());
		itkResult->Update();

		vtkResult->SetInput(itkResult->GetOutput());
		vtkResult->Update();
	}

	if (imageStyle->GetFlag() == 2 || imageStyle->GetFlag() == 3)
	{
		int z = IS_MyCanvas3D->GetCurrentSlice();

		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1], 0);
		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		double* position = Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint();

		IS_MyCanvas3D->Arc(position[0], position[1], z, 1, vtkResult->GetOutput());
	}
	
	vtkResult->Update();

	auto blend = vtkSmartPointer<vtkImageBlend>::New();
	blend->AddInputData(vtkResult->GetOutput());
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

VTKImageToImageType::Pointer MyCallback3D::GetFinalResult()
{
	itkFinalResult->SetInput(vtkResult->GetOutput());
	itkFinalResult->Update();
	return itkFinalResult;
}
