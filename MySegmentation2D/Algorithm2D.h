#pragma once

#include<vector>
#include "vtkObjectFactory.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkRGBPixel.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "Canvas2D.h"
#include "MySpeedFunction2D.h"
#include "ImageTypeDetails2D.h"



typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType    NodeType;


class Algorithm2D
{
public:

	Algorithm2D();
	void set_Canvas(Canvas2D*);
	void set_reader(itk::SmartPointer<ImageType_2_InternalImageType>);

	typedef MySpeedFunction2D< InternalImageType, InternalImageType > MySpeedFunction2DType;
	void Set_Function(itk::SmartPointer<MySpeedFunction2DType>);

	void FastMarching(const double);
	void Level_Set(int lower, int upper, double edge, double weight);
	void Level_Set(double edge, double weight);

	int WriteImage(std::string output1);

	InternalImageType* Get_FastMarching();
	OutputImageType*  Get_thresholder();


private:

	ImageType_2_InternalImageType::Pointer reader = ImageType_2_InternalImageType::New();
	WriterType::Pointer writer1 = WriterType::New();

	Canvas2D* diagram;
	MySpeedFunction2DType::Pointer my_function = MySpeedFunction2DType::New();
	
	typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType>
		ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

	NodeContainer::Pointer seeds = NodeContainer::New();
	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();

	typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType,
		InternalImageType > ThresholdSegmentationLevelSetImageFilterType;
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
		ThresholdSegmentationLevelSetImageFilterType::New();
};