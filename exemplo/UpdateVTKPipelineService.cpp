#include "UpdateVTKPipelineService.h"

UpdateVTKPipelineService::UpdateVTKPipelineService(TStencilFilter::Pointer StencilFilter, 
												   vtkSmartPointer<vtkImageAlgorithm> VtkImporter, 
												   vtkSmartPointer<vtkVolumeMapper> Mapper){
	this->Mapper = Mapper;
	this->VtkImporter = VtkImporter;
	this->StencilFilter = StencilFilter;
}

void UpdateVTKPipelineService::Execute(){
	StencilFilter->Update();
	VtkImporter->Modified();
	VtkImporter->Update();
	Mapper->Update();
}