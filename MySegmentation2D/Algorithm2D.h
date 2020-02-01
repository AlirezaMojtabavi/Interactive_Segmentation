#pragma once

#include<vector>
#include "vtkObjectFactory.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkRGBPixel.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "Canvas2D.h"
#include "MySpeedFunction2D.h"
#include "ImageTypeDetails2D.h"


class Algorithm2D
{
public:

	Algorithm2D();
	void set_reader(itk::SmartPointer<ImageType_2_InternalImageType> _IS2D_InternalImage);
	void set_Canvas(Canvas2D*);
	void Set_Function(itk::SmartPointer<MySpeedFunction2DType> _Function);

	void FastMarching(const double);
	void Level_Set(int lower, int upper, double edge, double weight);
	void Level_Set(double edge, double weight);

	OutputImageType*  GetResult();

private:

	ImageType_2_InternalImageType::Pointer IS2D_InternalImage = ImageType_2_InternalImageType::New();
	Canvas2D* diagram;
	NodeContainer::Pointer seeds = NodeContainer::New();
	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();
	MySpeedFunction2DType::Pointer mySpeedFunction = MySpeedFunction2DType::New();
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
		ThresholdSegmentationLevelSetImageFilterType::New();
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
};