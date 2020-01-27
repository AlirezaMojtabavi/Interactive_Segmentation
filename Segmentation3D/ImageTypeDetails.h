#pragma once
#include <itkImage.h>


static const  unsigned int  Dimension = 3;
typedef  float  InternalPixelType;
typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

typedef itk::Image < unsigned short, Dimension > ImageType;

typedef unsigned short OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;


