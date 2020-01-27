
#pragma once
#include <vector>
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include "vtkImageActor.h"
#include "math.h"
#include <vtkRegularPolygonSource.h>
#include <vtkPolyDataMapper.h>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include <vtkProperty.h>
#include "MyInteractionStyle3D.h"
#include "ImageTypeDetails.h"


struct coordinate3D
{
	double _x;
	double _y;
	int _z;
};

typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;
typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalType;

class MyCanvas3D
{
public:

	MyCanvas3D(vtkImageReslice* Reslice3D) :IS_ResliceDicom3D(Reslice3D) {}
	void SetImageData(vtkSmartPointer<vtkImageData> image);
	void SetRenderer(vtkSmartPointer<vtkRenderer>);
	void SetWindow(vtkSmartPointer<vtkRenderWindow>);
	void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);
	void SetStyle(MyInteractionStyle3D*);

	void Arc(double x, double y, int z, int r, vtkImageReslice* IS_OutPut); // for manual refining result
	void SetLastposition(double x, double y, int z);
	int paint(int x, int y, int z); // show seeds on window
	double* GetMinMaxIntensity();
	int GetCurrentSlice();
	vtkSmartPointer<vtkImageData> GetImage();
	std::vector<coordinate3D> GetVectorOfPoints();
	std::vector<vtkSmartPointer<vtkActor>> GetActor();

	void MouseWheelForward();
	void MouseWheelBackward();

private:

	vtkSmartPointer<vtkImageData> data = vtkSmartPointer<vtkImageData>::New();
	vtkImageReslice* IS_ResliceDicom3D ;
	std::vector<coordinate3D>PointList;
	MyInteractionStyle3D* style;

	int current;
	double* pSpacing;
	std::vector<double>Intensity;
	std::vector<double>min;
	std::vector<double>max;
	double threshold[2];

	std::vector<vtkSmartPointer<vtkActor>>ActorList;

	vtkSmartPointer<vtkRenderer>renderer;
	vtkSmartPointer<vtkRenderWindow>window;
	vtkSmartPointer<vtkRenderWindowInteractor>interactor;
};

