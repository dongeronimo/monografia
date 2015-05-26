#ifndef __update_vtk_pipe_h
#define __update_vtk_pipe_h
#include "ItkTypedefs.h"
#include <vtkImageAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkVolumeMapper.h>
class UpdateVTKPipelineService{
private:
	TStencilFilter::Pointer StencilFilter;
	vtkSmartPointer<vtkImageAlgorithm> VtkImporter;
	vtkSmartPointer<vtkVolumeMapper> Mapper;
public:
	UpdateVTKPipelineService(TStencilFilter::Pointer StencilFilter, 
							 vtkSmartPointer<vtkImageAlgorithm> VtkImporter, 
							 vtkSmartPointer<vtkVolumeMapper> Mapper);
	void Execute();
};

#endif