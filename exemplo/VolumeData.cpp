#include "VolumeData.h"
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

VolumeData LoadFromFile(std::string dir){
	vtkSmartPointer<vtkDICOMImageReader> Reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	Reader->SetDirectoryName(dir.c_str());
	Reader->Update();
	vtkSmartPointer<vtkImageData> img = Reader->GetOutput();
	VolumeData d;
	d.CenterX = img->GetCenter()[0];
	d.CenterY = img->GetCenter()[1];
	d.CenterZ = img->GetCenter()[2];
	d.SizeX = img->GetExtent()[1] + 1;
	d.SizeY = img->GetExtent()[3] + 1;
	d.SizeZ = img->GetExtent()[5] + 1;
	d.SpacingX = img->GetSpacing()[0];
	d.SpacingY = img->GetSpacing()[1];
	d.SpacingZ = img->GetSpacing()[2];
	d.Data = 0;
	size_t sz = d.SizeX * d.SizeY * d.SizeZ * sizeof(short);
	d.Data = (short*)malloc(sz);
	memcpy(d.Data, img->GetScalarPointer(), sz);
	return d;
}