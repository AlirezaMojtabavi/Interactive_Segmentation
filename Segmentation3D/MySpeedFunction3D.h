
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
	typedef itk::SmartPointer< Self >       Pointer;
	typedef itk::SmartPointer< const Self > ConstPointer;
	itkNewMacro(Self);
	itkTypeMacro(MySpeedFunction3D, itk::ThresholdSegmentationLevelSetFunction);

	MySpeedFunction3D() {}
	~MySpeedFunction3D() {}

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


	MySpeedFunction3D(const Self &) ITK_DELETE_FUNCTION;
	itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

	typename Superclass::ScalarValueType max_scale;
	typename Superclass::PixelType th, lap;

};

template<typename TImageType, typename TFeatureImageType>
inline void MySpeedFunction3D<TImageType, TFeatureImageType>::CalculateSpeedImage()
{

	cout << "\n\n" << "MySpeed3D is on" << "\n\n";

	typedef itk::GradientAnisotropicDiffusionImageFilter<TFeatureImageType, TFeatureImageType >  GradientType;
	GradientType::Pointer diffusion = GradientType::New();

	typedef itk::LaplacianImageFilter<TFeatureImageType, TFeatureImageType >  LaplacianType;
	LaplacianType::Pointer laplacian = LaplacianType::New();

	typedef itk::MinimumMaximumImageCalculator <FeatureImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter
		= ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(this->GetFeatureImage());
	imageCalculatorFilter->Compute();
	ScalarValueType max_image = imageCalculatorFilter->GetMaximum();


	typedef itk::InvertIntensityImageFilter< TFeatureImageType, TFeatureImageType > InvertType;
	InvertType::Pointer inverter = InvertType::New();
	inverter->SetInput(this->GetFeatureImage());
	inverter->SetMaximum(max_image);
	inverter->Update();

	typedef  itk::ImageFileWriter<  TFeatureImageType  > WriterType1;

	itk::ImageRegionIterator< FeatureImageType > lit;
	itk::ImageRegionConstIterator< FeatureImageType >
		fit(this->GetFeatureImage(), this->GetFeatureImage()->GetRequestedRegion());
	itk::ImageRegionIterator< ImageType >
		sit(this->GetSpeedImage(), this->GetFeatureImage()->GetRequestedRegion());


	ImageCalculatorFilterType::Pointer imageCalculatorFilter1
		= ImageCalculatorFilterType::New();
	ScalarValueType max_lap;
	ScalarValueType min_lap;
	ScalarValueType max_lap_abs;

	//----------------------------------------------------------------------------------

	//typedef itk::RescaleIntensityImageFilter< InternalImageType, OutputImageType >
	//	CastFilterType;
	//typedef   itk::CurvatureAnisotropicDiffusionImageFilter< InternalImageType, InternalImageType >
	//	SmoothingFilterType;
	//typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter< InternalImageType, InternalImageType > GradientFilterType;

	//SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();
	//GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();

	//---------------------------------------------------------------------------------


	if (m_EdgeWeight != 0.0)
	{
		diffusion->SetInput(inverter->GetOutput());
		diffusion->SetConductanceParameter(8);
		diffusion->SetTimeStep(0.03);
		diffusion->SetNumberOfIterations(9);
		diffusion->Update();

		laplacian->SetInput(diffusion->GetOutput());
		laplacian->Update();

		imageCalculatorFilter1->SetImage(laplacian->GetOutput());
		imageCalculatorFilter1->Compute();
		max_lap = imageCalculatorFilter1->GetMaximum();
		min_lap = imageCalculatorFilter1->GetMinimum();
		//cout << "the maximum of laplacian: " << max_lap << "\n";
		//cout << "the minimum of laplacian: " << min_lap << "\n\n";

		//-------------------------------------------------------------------


		lit = itk::ImageRegionIterator< FeatureImageType >
			(laplacian->GetOutput(), this->GetFeatureImage()->GetRequestedRegion());
		lit.GoToBegin();
	}

	// Copy the meta information (spacing and origin) from the feature image
	this->GetSpeedImage()->CopyInformation(this->GetFeatureImage());

	// Calculate the speed image
	ScalarValueType upper_threshold = static_cast<ScalarValueType>(m_UpperThreshold);
	ScalarValueType lower_threshold = static_cast<ScalarValueType>(m_LowerThreshold);
	ScalarValueType mid = ((upper_threshold - lower_threshold) / 2.0) + lower_threshold;
	ScalarValueType threshold;

	for (fit.GoToBegin(), sit.GoToBegin(); !fit.IsAtEnd(); ++sit, ++fit)
	{
		if (static_cast<ScalarValueType>(fit.Get()) < mid)
		{
			threshold = fit.Get() - lower_threshold;
			th = threshold;
		}
		else
		{
			threshold = upper_threshold - fit.Get();
			th = threshold;
		}

		if (m_EdgeWeight != 0.0)
		{
			max_scale = mid - m_LowerThreshold;

			sit.Set(static_cast<ScalarValueType>(threshold + m_EdgeWeight * lit.Get()));
			lap = lit.Get();
			++lit;
		}
		else
		{
			sit.Set(static_cast<ScalarValueType>(threshold));

		}
	}

	//-----------------------------------------------	Histogram laplacian	----------------------------------------------

	//ImageCalculatorFilterType::Pointer imageCalculatorFilter2
	//	= ImageCalculatorFilterType::New();
	//imageCalculatorFilter2->SetImage(laplacian->GetOutput());
	//imageCalculatorFilter2->Compute();
	//float max_speed = imageCalculatorFilter2->GetMaximum();
	//float min_speed = imageCalculatorFilter2->GetMinimum();
	//cout << "the maximum of speed function: " << max_speed << "\n";
	//cout << "the minimum of speed function: " << min_speed << "\n\n";

	//const unsigned int MeasurementVectorSize = 1; // Grayscale
	//const unsigned int binsPerDimension =
	//	static_cast< unsigned int >(max_speed - min_speed +1);

	//typedef itk::Statistics::ImageToHistogramFilter< FeatureImageType >
	//	ImageToHistogramFilterType;
	//ImageToHistogramFilterType::HistogramType::MeasurementVectorType
	//	lowerBound(binsPerDimension);
	//lowerBound.Fill(min_speed);
	//ImageToHistogramFilterType::HistogramType::MeasurementVectorType
	//	upperBound(binsPerDimension);
	//upperBound.Fill(max_speed);
	//ImageToHistogramFilterType::HistogramType::SizeType
	//	size(MeasurementVectorSize);
	//size.Fill(binsPerDimension);

	//ImageToHistogramFilterType::Pointer imageToHistogramFilter =
	//	ImageToHistogramFilterType::New();
	//imageToHistogramFilter->SetInput(laplacian->GetOutput());
	//imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
	//imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
	//imageToHistogramFilter->SetHistogramSize(size);

	//imageToHistogramFilter->Update();
	//
	//ImageToHistogramFilterType::HistogramType* histogram =
	//	imageToHistogramFilter->GetOutput();

	//ofstream outFile;
	//outFile.open("lap histogram.txt");

	//outFile<< "Frequency = [ ";
	//
	//for (unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
	//{
	//	outFile << i + min_speed << " : ";
	//	outFile << histogram->GetFrequency(i);
	//	if (i != histogram->GetSize()[0] - 1)
	//	{
	//		outFile << "," << '\n';
	//	}
	//}
	//outFile << " ]" << endl;
	//
	//outFile.close();


	////-------------------------------- My Histogram ------------------------------ 

	//ImageCalculatorFilterType::Pointer imageCalculatorFilter3
	//	= ImageCalculatorFilterType::New();
	//imageCalculatorFilter3->SetImage(sit.GetImage());
	//imageCalculatorFilter3->Compute();
	//float max_speed2 = imageCalculatorFilter3->GetMaximum();
	//float min_speed2 = imageCalculatorFilter3->GetMinimum();
	//cout << "the maximum of speed function: " << max_speed << "\n";
	//cout << "the minimum of speed function: " << min_speed << "\n\n";

	//const unsigned int binsPerDimension2 =
	//	static_cast< unsigned int >(max_speed2 - min_speed2 + 1);

	//ImageToHistogramFilterType::HistogramType::MeasurementVectorType
	//	lowerBound2(binsPerDimension2);
	//lowerBound2.Fill(min_speed2);
	//ImageToHistogramFilterType::HistogramType::MeasurementVectorType
	//	upperBound2(binsPerDimension2);
	//upperBound2.Fill(max_speed2);
	//ImageToHistogramFilterType::HistogramType::SizeType
	//	size2(MeasurementVectorSize);
	//size2.Fill(binsPerDimension2);

	//ImageToHistogramFilterType::Pointer imageToHistogramFilter2 =
	//	ImageToHistogramFilterType::New();
	//imageToHistogramFilter2->SetInput(sit.GetImage());
	//imageToHistogramFilter2->SetHistogramBinMinimum(lowerBound2);
	//imageToHistogramFilter2->SetHistogramBinMaximum(upperBound2);
	//imageToHistogramFilter2->SetHistogramSize(size2);

	//imageToHistogramFilter2->Update();

	//ImageToHistogramFilterType::HistogramType* histogram2 =
	//	imageToHistogramFilter2->GetOutput();

	//ofstream outFile2;
	//outFile2.open("my histogram.txt");

	//outFile2 << "Frequency = [ ";

	//for (unsigned int i = 0; i < histogram2->GetSize()[0]; ++i)
	//{
	//	outFile2 << i + min_speed << " : ";
	//	outFile2 << histogram2->GetFrequency(i);
	//	if (i != histogram2->GetSize()[0] - 1)
	//	{
	//		outFile << "," << '\n';
	//	}
	//}
	//outFile2 << " ]" << endl;

	//outFile2.close();

}


#ifndef ITK_MANUAL_INSTANTIATION

#endif

#endif