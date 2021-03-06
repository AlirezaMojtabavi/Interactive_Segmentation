#include "stdafx.h"
#include "Canvas2D.h"


Canvas2D::Canvas2D()
{}

void Canvas2D::SetImageData(vtkSmartPointer<vtkImageData> _image)
{
	data->ShallowCopy(_image);
	pSpacing = data->GetSpacing();
}

std::vector<coordinate> Canvas2D::GetPointList()
{
	return PointList;
}

void Canvas2D::SetStyle(InteractorStyle2D* _style)
{
	style = _style;
}

void Canvas2D::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void Canvas2D::SetWindow(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

void Canvas2D::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> _interactor)
{
	interactor = _interactor;
}

vtkImageData* Canvas2D::GetImage()
{
	return data;
}

double* Canvas2D::get_min_max()
{
	if (!*threshold)
	{
		threshold[0] = Intensity[0];
		threshold[1] = Intensity[0];
	}

	int size = Intensity.size();

	for (int i = 0; i < size; i++)
	{
		if (Intensity[i] <= threshold[0])
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

	if (style->GetFlag() == 5)
	{
		min.pop_back();
		max.pop_back();
	}

	threshold[0] = min.back();
	threshold[1] = max.back();

	return threshold;
}

void Canvas2D::Arc(double x, double y, int r, vtkImageData* inputData)
{
	for (int i = 0; i < r + 1; i++)
	{
		for (int theta = 0; theta < 361; theta++)
		{
			double deg = theta*0.0174532925;
			double X = i*cos(deg) + x;
			double Y = i*sin(deg) + y;
			unsigned short* pixel = static_cast<unsigned short*> (inputData->GetScalarPointer(X / pSpacing[0], Y / pSpacing[1], 0));
			if (!pixel)
				return;

			if (style->GetFlag() == 2)
				pixel[0] = 255;

			if (style->GetFlag() == 3)
				pixel[0] = 0;

			inputData->Modified();

		}
	}
}

int Canvas2D::paint(int x, int y)
{
	auto polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
	polygonSource->SetNumberOfSides(50);
	polygonSource->SetRadius(1);
	polygonSource->SetCenter(x, y, 0);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(polygonSource->GetOutputPort());

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(1, 0, 0);
	ActorList.push_back(actor);

	renderer->AddActor(ActorList.back());
	window->AddRenderer(renderer);
	interactor->SetRenderWindow(window);

	return EXIT_SUCCESS;
}


void Canvas2D::SetLastposition(double x, double y)
{
	Arc(x, y, 1, data);
	coordinate MySeed;

	cout << "value:  " << data->GetScalarComponentAsDouble((int)x / pSpacing[0], (int)y / pSpacing[1], 0, 0) << "\n\n";
	//cout << "x: " << x << "\t\t\ty: " << y << endl;
	MySeed._x = x / pSpacing[0];
	MySeed._y = y / pSpacing[1];

	if (style->GetFlag() == 5) // remove last seed
	{
		PointList.pop_back();
		Intensity.pop_back();
		min.pop_back();
		max.pop_back();

		renderer->RemoveActor(ActorList.back());
		window->AddRenderer(renderer);
		interactor->SetRenderWindow(window);
		ActorList.pop_back();
	}

	else if (style->GetFlag() != 2 && style->GetFlag() != 3)
	{
		paint(x, y);
		PointList.push_back(MySeed);
		Intensity.push_back(data->GetScalarComponentAsDouble((int)x / pSpacing[0], (int)y / pSpacing[1], 0, 0));
	}

	interactor->Render();

	return;
}

