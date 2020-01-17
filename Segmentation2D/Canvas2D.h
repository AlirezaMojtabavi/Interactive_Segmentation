#pragma once
#include<vector>
#include "InteractorStyle2D.h"
#include "vtkSmartPointer.h"
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



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

struct coordinate
{
	double _x;
	double _y;
};

class Canvas2D
{
public:

	Canvas2D();
	void Arc(double x, double y, int r, vtkImageData* data);
	void SetLastposition(double x, double y);
	std::vector<coordinate> get_vector();
	int paint(int x, int y);

	void SetImageData(vtkSmartPointer<vtkImageData>);
	void set_style(InteractorStyle2D*);
	void set_renderer(vtkSmartPointer<vtkRenderer>);
	void set_window(vtkSmartPointer<vtkRenderWindow>);
	void set_interactor(vtkSmartPointer<vtkRenderWindowInteractor>);

	double* get_min_max();
	vtkImageData* getImage();


private:

	VTK_CREATE(vtkImageData, data);
	std::vector<vtkSmartPointer<vtkActor>>ActorList;
	std::vector<double>Intensity;
	std::vector<double>min;
	std::vector<double>max;
	double threshold[2];

	std::vector<coordinate>PointList;

	InteractorStyle2D* style;
	VTK_CREATE(vtkRenderWindowInteractor, interactor);
	VTK_CREATE(vtkRenderer, renderer);
	VTK_CREATE(vtkRenderWindow, window);
};

