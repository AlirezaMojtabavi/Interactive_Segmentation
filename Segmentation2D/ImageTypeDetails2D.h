#pragma once
#include <itkImage.h>


static const  unsigned int  Dimension = 2;
typedef  float  InternalPixelType2D;
typedef  itk::Image< InternalPixelType2D, Dimension >  InternalImageType2D ;

typedef itk::Image < unsigned short, Dimension > ImageType2D;

typedef unsigned short OutputPixelType2D;
typedef itk::Image< OutputPixelType2D, Dimension > OutputImageType2D;

