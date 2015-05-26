#ifndef __itk_typedefs_h
#define __itk_typedefs_h
#include <itkImage.h>
#include <itkImportImageFilter.h>
#include <itkImageSource.h>
#include "MyItkStencil.h"

typedef itk::Image<short, 3> TImage;
typedef itk::Image<bool, 3> TMask;
typedef itk::ImageSource<TMask> TMaskSource;
typedef itk::ImportImageFilter<short, 3> TItkImageImport;
typedef itk::ImageSource<TImage> TImageSource;
typedef itk::MyItkStencil<TImage> TStencilFilter;

#endif