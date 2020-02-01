#pragma once
#include "itkImage.h"
#include <itkCastImageFilter.h>

const  unsigned int  Dimension = 2;
typedef itk::Image < unsigned short, Dimension > ImageType;

typedef  float  InternalPixelType;
typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

typedef unsigned short OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;
typedef itk::ImageToVTKImageFilter<ImageType>  ConnectorType;

typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;

typedef itk::CastImageFilter<ImageType, InternalImageType> ImageType_2_InternalImageType;
typedef itk::CastImageFilter<InternalImageType, OutputImageType> InternalImageType_2_OutputImageType;
typedef itk::CastImageFilter<OutputImageType, ImageType> OutputImageType_2_ImageType;