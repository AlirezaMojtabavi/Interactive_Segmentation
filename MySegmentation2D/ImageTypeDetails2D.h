#pragma once
#include "itkImage.h"
#include <itkCastImageFilter.h>

//--------------------------Defenition Image Type --------------------------------------

const  unsigned int  Dimension = 2;
typedef itk::Image < unsigned short, Dimension > ImageType;

typedef  float  InternalPixelType;
typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

typedef unsigned short OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType; //convert vtk image to itk image
typedef itk::ImageToVTKImageFilter<ImageType>  ConnectorType; //convert itk image to vtk image

//--------------------------Defenition Algorithm Type --------------------------------------

typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
typedef FastMarchingFilterType::NodeContainer   NodeContainer;
typedef FastMarchingFilterType::NodeType  NodeType;

typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType,// for sparse field levelset algorithm
	InternalImageType > ThresholdSegmentationLevelSetImageFilterType;
typedef MySpeedFunction2D< InternalImageType, InternalImageType > MySpeedFunction2DType; // for speed function(cost function)
typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType> // for output image
ThresholdingFilterType;

typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType; // for overlay

//--------------------------Defenition CastImage Type --------------------------------------

typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalImageType;
typedef itk::CastImageFilter<InternalImageType, OutputImageType> InternalImageType_2_OutputImageType;
typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;