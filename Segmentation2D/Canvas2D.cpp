#include "stdafx.h"
#include "Canvas2D.h"


//vtkStandardNewMacro(Canvas2D);

Canvas2D::Canvas2D()
{}

void Canvas2D::SetImageData(vtkSmartPointer<vtkImageData> image)
{
	data->ShallowCopy(image);
	double* pSpacing = data->GetSpacing();
}

std::vector<coordinate> Canvas2D::get_vector()
{
	return PointList;
}

void Canvas2D::set_style(InteractorStyle2D* _style)
{
	style = _style;
}

void Canvas2D::set_renderer(vtkSmartPointer<vtkRenderer> _renderer)
{
	renderer = _renderer;
}

void Canvas2D::set_window(vtkSmartPointer<vtkRenderWindow> _window)
{
	window = _window;
}

void Canvas2D::set_interactor(vtkSmartPointer<vtkRenderWindowInteractor> _interactor)
{
	interactor = _interactor;
}

vtkImageData* Canvas2D::getImage()
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

	if (style->GetFlag() == 7)
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
	double* pSpacing = inputData->GetSpacing();
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

	VTK_CREATE(vtkRegularPolygonSource, polygonSource);
	polygonSource->SetNumberOfSides(50);
	polygonSource->SetRadius(1);
	polygonSource->SetCenter(x, y, 0);

	VTK_CREATE(vtkPolyDataMapper, mapper);
	mapper->SetInputConnection(polygonSource->GetOutputPort());

	VTK_CREATE(vtkActor, actor);
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
	double* pSpacing = data->GetSpacing();

	Arc(x, y, 1, data);
	coordinate MySeed;

	cout << "value:  " << data->GetScalarComponentAsDouble((int)x / pSpacing[0], (int)y / pSpacing[1], 0, 0) << "\n\n";
	//cout << "x: " << x << "\t\t\ty: " << y << endl;
	MySeed._x = x / pSpacing[0];
	MySeed._y = y / pSpacing[1];


	if (style->GetFlag() == 7)
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

