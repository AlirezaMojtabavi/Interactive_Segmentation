
#pragma once

#ifndef MySpeedFunction3D_h
#define MySpeedFunction3D_h

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
class MySpeedFunction3D :
	public itk::ThresholdSegmentationLevelSetFunction< TImageType, TFeatureImageType >
{

public:

	typedef TFeatureImageType      FeatureImageType;
	typedef double  PixelRealType;

	typedef MySpeedFunction3D Self;
	typedef itk::ThresholdSegmentationLevelSetFunction< TImageType, TFeatureImageType > Superclass;
	typedef itk::SmartPointer< Self >  Pointer;
	typedef itk::SmartPointer< const Self > ConstPointer;
	itkNewMacro(Self);
	itkTypeMacro(MySpeedFunction3D, itk::ThresholdSegmentationLevelSetFunction);

	MySpeedFunction3D() {}
	~MySpeedFunction3D() {}

	void CalculateSpeedImage() override;
	virtual const FeatureImageType * GetFeatureImage() const
	{
		return m_FeatureImage.GetPointer();
	}
	virtual ImageType * GetSpeedImage()
	{
		return m_SpeedImage.GetPointer();
	}


protected:


	MySpeedFunction3D(const Self &) ITK_DELETE_FUNCTION;
	itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

	typename Superclass::ScalarValueType max_scale;
	typename Superclass::PixelType th, lap;

};


#ifndef ITK_MANUAL_INSTANTIATION

#endif

#endif
