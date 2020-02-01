#pragma once
#include<vector>
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "math.h"
#include <vtkRegularPolygonSource.h>
#include <vtkPolyDataMapper.h>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include <vtkProperty.h>
#include "InteractorStyle2D.h"


struct coordinate
{
	double _x;
	double _y;
};

class Canvas2D
{
public:

	Canvas2D();

	void SetImageData(vtkSmartPointer<vtkImageData>);
	void set_style(InteractorStyle2D*);
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);
	void set_interactor(vtkSmartPointer<vtkRenderWindowInteractor>);

	void Arc(double x, double y, int r, vtkImageData* data);
	void SetLastposition(double x, double y);
	std::vector<coordinate> get_vector();
	int paint(int x, int y);

	double* get_min_max();
	vtkImageData* getImage();

private:

	vtkSmartPointer<vtkImageData> data = vtkSmartPointer<vtkImageData>::New();
	double* pSpacing;
	std::vector<vtkSmartPointer<vtkActor>>ActorList;
	std::vector<double>Intensity;
	std::vector<double>min;
	std::vector<double>max;
	double threshold[2];

	std::vector<coordinate>PointList;

	InteractorStyle2D* style;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> window;
};

