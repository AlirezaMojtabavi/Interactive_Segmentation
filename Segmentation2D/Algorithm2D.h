#pragma once

#include<vector>
#include "vtkSmartPointer.h"
#include "vtkObjectFactory.h"
#include <itkImage.h>
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingImageFilter.h"
#include <itkCastImageFilter.h>
#include "itkSmartPointer.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include <vtkImageBlend.h>
#include "itkRGBPixel.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include <algorithm> 

#include "Canvas2D.h"
#include "SpeedFunction2D.h"
#include "ImageTypeDetails2D.h"


//-------------------------Type Of Images--------------------------
//-----------------------------------------------------------------


typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
typedef SpeedFunction2D< InternalImageType, InternalImageType > MySpeedType;

//-----------------------------------------------------------------
//-------------------------Alorithm--------------------------------
//-----------------------------------------------------------------

typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType    NodeType;

typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType,
	InternalImageType > ThresholdSegmentationLevelSetImageFilterType;
typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType>
ThresholdingFilterType;

typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalImageType;
typedef itk::CastImageFilter<InternalImageType, OutputImageType> InternalImageType_2_OutputImageType;
typedef itk::CastImageFilter<OutputImageType, InternalImageType> OutputImageType_2_InternalImageType;


class Algorithm2D
{
public:

	Algorithm2D();
	void set_Canvas(Canvas2D*);
	void set_reader(itk::SmartPointer<InternalImageType >);

	void Set_Function(itk::SmartPointer<MySpeedType>);
	void FastMarching(const double);
	void Level_Set(int lower, int upper, double edge, double weight);
	void Level_Set(double edge, double weight);

	int WriteImage(std::string output1);

	InternalImageType* Get_FastMarching();
	OutputImageType* Get_thresholder();


private:

	InternalImageType::Pointer reader = InternalImageType::New();
	WriterType::Pointer writer1 = WriterType::New();

	Canvas2D* diagram;
	MySpeedType::Pointer my_function = MySpeedType::New();

	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
	NodeContainer::Pointer seeds = NodeContainer::New();
	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
		ThresholdSegmentationLevelSetImageFilterType::New();
};