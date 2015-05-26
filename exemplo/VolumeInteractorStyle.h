#ifndef __volume_interaction_h
#define __volume_interaction_h
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkImageStencil.h>
#include <vector>
#include "PontoNaTela.h"
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>
#include <string>
#include "SolidoDeExtrusao.h"
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkImageStencilData.h>
#include <vtkImageData.h>
#include "UpdateVTKPipelineService.h"
#include <vtkImageAlgorithm.h>
#include <vtkVolumeMapper.h>

using namespace std;

class VolumeInteractionStyle : public vtkInteractorStyleTrackballCamera{
private:
	vector<PontoNaTela> Pontos;
	vtkSmartPointer<vtkImageData> Image;
	TStencilFilter::Pointer StencilFilter;
	vtkSmartPointer<vtkImageAlgorithm> VtkImporter;
	vtkSmartPointer<vtkVolumeMapper> Mapper;
protected:
	VolumeInteractionStyle(){
		Image = 0;
		StencilFilter = 0;
		VtkImporter = 0;
		Mapper = 0;
	}
public:
	static VolumeInteractionStyle* New();
	vtkTypeMacro(VolumeInteractionStyle, vtkInteractorStyleTrackballCamera);
	void SetImageData(vtkSmartPointer<vtkImageData> img){Image = img;}
	void SetStencilFilter(TStencilFilter::Pointer stencil){StencilFilter = stencil;}
	void SetMapper(vtkSmartPointer<vtkVolumeMapper> m){Mapper = m;}
	void SetVTKImporter(vtkSmartPointer<vtkImageAlgorithm> i){VtkImporter = i;}
	void OnKeyDown();
};

#endif