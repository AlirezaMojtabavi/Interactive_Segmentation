#pragma once

#ifndef MySpeedFunction2D_h
#define MySpeedFunction2D_h

#include <itkImage.h>
#include <itkMacro.h>
#include "itkThresholdSegmentationLevelSetFunction.h"
#include "itkImageRegionIterator.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkLaplacianImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkHistogram.h"
#include <itkImageToHistogramFilter.h>
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"


template< typename TImageType, typename TFeatureImageType = TImageType >
class MySpeedFunction2D :
	public itk::ThresholdSegmentationLevelSetFunction< TImageType, TFeatureImageType >
{

public:

	typedef TFeatureImageType      FeatureImageType;
	typedef double                        PixelRealType;

	typedef MySpeedFunction2D Self;
	typedef itk::ThresholdSegmentationLevelSetFunction< TImageType, TFeatureImageType > Superclass;
	typedef itk::SmartPointer< Self >       Pointer;
	typedef itk::SmartPointer< const Self > ConstPointer;
	itkNewMacro(Self);
	itkTypeMacro(MySpeedFunction2D, itk::ThresholdSegmentationLevelSetFunction);

	virtual void CalculateSpeedImage() override;
	virtual const FeatureImageType * GetFeatureImage() const
	{
		return m_FeatureImage.GetPointer();
	}
	virtual ImageType * GetSpeedImage()
	{
		return m_SpeedImage.GetPointer();
	}



protected:

	MySpeedFunction2D() {}
	~MySpeedFunction2D() {}
	MySpeedFunction2D(const Self &) ITK_DELETE_FUNCTION;
	itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);
	//typedef double                        PixelRealType;
	//PixelRealType m_ScaleCoefficients[ImageDimension];

	ScalarValueType max_scale;
	PixelType th, lap;

};



#ifndef ITK_MANUAL_INSTANTIATION

#endif

#endif
