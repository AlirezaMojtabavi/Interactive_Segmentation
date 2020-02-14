
#include "stdafx.h"
#include "MyCanvas3D.h"



void MyCanvas3D::SetImageData(vtkSmartPointer<vtkImageData> image)
{
	data->ShallowCopy(image);
	pSpacing = data->GetSpacing();
}

void MyCanvas3D::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void MyCanvas3D::SetWindow(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

void MyCanvas3D::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> _interactor)
{
	interactor = _interactor;
}


void MyCanvas3D::Arc(double x, double y, int z, int r, vtkSmartPointer<vtkImageData> inputData)
{
	for (int i = 0; i < r + 1; i++)
	{
		for (int theta = 0; theta < 361; theta++)
		{
			double deg = theta*0.0174532925;
			double X = i*cos(deg) + x;
			double Y = i*sin(deg) + y;
			unsigned short* pixel = static_cast<unsigned short*> (inputData->GetScalarPointer(X / pSpacing[0], Y / pSpacing[1], z));
			if (!pixel)
				return;

			if (style->GetFlag() == 2) // refine image
				pixel[0] = 255;

			if (style->GetFlag() == 3) // erase image
				pixel[0] = 0;

			inputData->Modified();

		}
	}
}


int MyCanvas3D::paint(int x, int y, int z)
{
	if (z != this->GetCurrentSlice())
		return EXIT_FAILURE;

	auto polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
	polygonSource->SetNumberOfSides(50);
	polygonSource->SetRadius(1);
	polygonSource->SetCenter(x, y, z);
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(polygonSource->GetOutputPort());

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0, 0, 1);
	ActorList.push_back(actor);

	renderer->AddActor(ActorList.back());
	window->AddRenderer(renderer);
	interactor->SetRenderWindow(window);

	return EXIT_SUCCESS;
}


void MyCanvas3D::SetLastposition(double x, double y, int z)
{
	
	if (z != this->GetCurrentSlice())
		return;

	coordinate3D MySeed;

	cout << "VALUE:  " << IS_ResliceDicom3D->GetOutput()->GetScalarComponentAsDouble((int)x / pSpacing[0], (int)y / pSpacing[1], z, 0) << "\n\n";

	MySeed._x = x / pSpacing[0];
	MySeed._y = y / pSpacing[1];
	MySeed._z = z;

	if (style->GetFlag() == 5)
	{
		PointList.pop_back();
		Intensity.pop_back();

		renderer->RemoveActor(ActorList.back());
		window->AddRenderer(renderer);
		interactor->SetRenderWindow(window);
		ActorList.pop_back();
	}

	else if (style->GetFlag() != 2 && style->GetFlag() != 3)
	{
		paint(x, y, z);
		PointList.push_back(MySeed);
		Intensity.push_back(data->GetScalarComponentAsDouble((int)x / pSpacing[0], (int)y / pSpacing[1], z, 0));
	}
	interactor->Render();

	return;
}

std::vector<coordinate3D> MyCanvas3D::GetVectorOfPoints()
{
	return PointList;
}

std::vector<vtkSmartPointer<vtkActor>> MyCanvas3D::GetActor()
{
	return ActorList;
}

void MyCanvas3D::SetStyle(MyInteractionStyle3D * _style)
{
	style = _style;
}

double * MyCanvas3D::GetMinMaxIntensity()
{
	if (!*threshold)
	{
		threshold[0] = Intensity[0];
		threshold[1] = Intensity[0];
	}

	int size = Intensity.size();


	for (int i = 0; i < size; i++)
	{
		if (Intensity[i] <= threshold[0] && Intensity[i] > 100)
		{
			threshold[0] = Intensity[i];
			min.push_back(threshold[0]);
		}

		if (Intensity[i] >= threshold[1])
		{
			threshold[1] = Intensity[i];
			max.push_back(threshold[1]);
		}
	}

	threshold[0] = min.back();
	threshold[1] = max.back();

	return threshold;
}

int MyCanvas3D::GetCurrentSlice()
{
	return current;
}

vtkSmartPointer<vtkImageData> MyCanvas3D::GetImage()
{
	return data;
}

void MyCanvas3D::MouseWheelForward()
{
	vtkRenderWindowInteractor *interactor = this->interactor;
	double point[4];
	double center[4];
	vtkImageReslice *reslice = this->IS_ResliceDicom3D;

	int size = this->GetActor().size();

	for (int i = 0; i < size; i++)
		renderer->RemoveActor(this->GetActor()[i]);

	window->AddRenderer(renderer);
	interactor->SetRenderWindow(window);

	current++;

	if (current > reslice->GetImageDataInput(0)->GetExtent()[5])
	{
		current = reslice->GetImageDataInput(0)->GetExtent()[5];
		return;
	}
	reslice->UpdateInformation();
	double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
	vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
	// move the center point that we are slicing through
	point[0] = 0.0;
	point[1] = 0.0;
	point[2] = sliceSpacing;
	point[3] = 1.0;
	matrix->MultiplyPoint(point, center);
	matrix->SetElement(0, 3, center[0]);
	matrix->SetElement(1, 3, center[1]);
	matrix->SetElement(2, 3, center[2]);
	cout << "Slice Number= " << current << endl;
	interactor->Render();

}

void MyCanvas3D::MouseWheelBackward()
{
	vtkRenderWindowInteractor *interactor = this->interactor;
	double point[4];
	double center[4];
	vtkImageReslice *reslice = this->IS_ResliceDicom3D;

	int size = this->GetActor().size();
	for (int i = 0; i < size; i++)
		renderer->RemoveActor(this->GetActor()[i]);

	window->AddRenderer(renderer);
	interactor->SetRenderWindow(window);

	current--;
	if (current == -1)
	{
		current = 0;
		return;
	}
	reslice->UpdateInformation();
	double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
	vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
	// move the center point that we are slicing through
	point[0] = 0.0;
	point[1] = 0.0;
	point[2] = -sliceSpacing;
	point[3] = 1.0;
	matrix->MultiplyPoint(point, center);
	matrix->SetElement(0, 3, center[0]);
	matrix->SetElement(1, 3, center[1]);
	matrix->SetElement(2, 3, center[2]);
	cout << "Slice Number= " << current << endl;
	interactor->Render();
}

