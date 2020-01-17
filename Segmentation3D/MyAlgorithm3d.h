
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




typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType    NodeType;



class MyAlgorithm3d
{
public:


	MyAlgorithm3d();
	void SetInternalImage(InternalImageType::Pointer _InternalImage);

	void SetCanvas(MyCanvas3D*);

	typedef MySpeedFunction3D< InternalImageType, InternalImageType > MySpeedFunction3DType;
	void SetSpeedFunction(itk::SmartPointer<MySpeedFunction3DType>);

	void FastMarching(const double);
	void LevelSet(int lower, int upper, double edge, double weight);
	void LevelSet(double edge, double weight);

	void SetStyle(MyInteractionStyle3D*);
	InternalImageType* GetFastMarching();
	OutputImageType*  GetThresholder();

private:

	InternalImageType::Pointer IS_InternalImage;
	vtkImageReslice* ImageReslice;
	MyCanvas3D* IS_MyCanvas3D;
	MyInteractionStyle3D* style;

	MySpeedFunction3DType::Pointer SegmentationSpeedFunction = MySpeedFunction3DType::New();

	typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType> ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

	typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType, InternalImageType >
		ThresholdSegmentationLevelSetImageFilterType;
	ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation = 
		ThresholdSegmentationLevelSetImageFilterType::New();


	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();
	NodeContainer::Pointer seeds = NodeContainer::New();
};