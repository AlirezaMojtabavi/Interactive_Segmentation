
#pragma once

#include<vector>
#include "vtkObjectFactory.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "MySpeedFunction3D.h"
#include "MyInteractionStyle3D.h"
#include "MyCanvas3D.h"
#include "ImageDetailsType.h"

typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType  NodeType;

class MyAlgorithm3d
{
public:

	MyAlgorithm3d();
	void SetInternalImage(InternalImageType::Pointer _InternalImage);

	void SetCanvas(MyCanvas3D*);
	void FastMarching(const double);
	void LevelSet(short int lower, short int upper, double edge, double weight);
	void LevelSet(double edge, double weight);

	InternalImageType* GetFastMarching();
	OutputImageType*  GetThresholder();

private:

	InternalImageType::Pointer IS_InternalImage;
	vtkImageReslice* ImageReslice;
	MyCanvas3D* IS_MyCanvas3D;

	typedef MySpeedFunction3D< InternalImageType, InternalImageType > MySpeedFunction3DType;
	MySpeedFunction3DType::Pointer SegmentationSpeedFunction = MySpeedFunction3DType::New();

	typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType> // for output image
		ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

	typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType, InternalImageType >
		ThresholdSegmentationLevelSetImageFilterType;
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation = 
		ThresholdSegmentationLevelSetImageFilterType::New();

	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();
	NodeContainer::Pointer seeds = NodeContainer::New();
};