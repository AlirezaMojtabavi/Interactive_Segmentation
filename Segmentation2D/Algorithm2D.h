#pragma once

#include<vector>
#include "vtkObjectFactory.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingImageFilter.h"
#include <itkCastImageFilter.h>
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include <vtkImageBlend.h>
#include "itkRGBPixel.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "Canvas2D.h"
#include "SpeedFunction2D.h"
#include "ImageTypeDetails2D.h"


//-------------------------Type Of Images--------------------------
//-----------------------------------------------------------------


typedef  itk::ImageFileWriter<  OutputImageType2D  > WriterType;


//-----------------------------------------------------------------
//-------------------------Alorithm--------------------------------
//-----------------------------------------------------------------

typedef  itk::FastMarchingImageFilter< InternalImageType2D, InternalImageType2D > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType    NodeType;


typedef itk::CastImageFilter<ImageType2D, InternalImageType2D> ImageType2D_2_InternalImageType2D;
typedef itk::CastImageFilter<InternalImageType2D, OutputImageType2D> InternalImageType2D_2_OutputImageType2D;
typedef itk::CastImageFilter<OutputImageType2D, InternalImageType2D> OutputImageType2D_2_InternalImageType2D;


class Algorithm2D
{
public:

	Algorithm2D();
	void set_reader(InternalImageType2D::Pointer);
	void set_Canvas(Canvas2D*);

	typedef SpeedFunction2D< InternalImageType2D, InternalImageType2D > MySpeedFunction2DType;
	void Set_Function(itk::SmartPointer<MySpeedFunction2DType>);
	void FastMarching(const double);
	void Level_Set(int lower, int upper, double edge, double weight);
	void Level_Set(double edge, double weight);

	int WriteImage(std::string output1);

	InternalImageType2D* Get_FastMarching();
	OutputImageType2D* Get_thresholder();


private:

	InternalImageType2D::Pointer reader;
	WriterType::Pointer writer1 = WriterType::New();

	Canvas2D* diagram;
	MySpeedFunction2DType::Pointer my_function = MySpeedFunction2DType::New();
	
	NodeContainer::Pointer seeds = NodeContainer::New();
	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();

	typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType2D,
		InternalImageType2D > ThresholdSegmentationLevelSetImageFilterType;
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
		ThresholdSegmentationLevelSetImageFilterType::New();

	typedef itk::BinaryThresholdImageFilter<InternalImageType2D, OutputImageType2D>
		ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
};