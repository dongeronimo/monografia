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
using namespace std;

class VolumeInteractionStyle : public vtkInteractorStyleTrackballCamera{
private:
	vector<PontoNaTela> Pontos;
	vtkSmartPointer<vtkImageData> Image;
protected:
	VolumeInteractionStyle(){
		Image = 0;
	}
public:
	static VolumeInteractionStyle* New();
	vtkTypeMacro(VolumeInteractionStyle, vtkInteractorStyleTrackballCamera);
	void SetImageData(vtkSmartPointer<vtkImageData> img){Image = img;}
	void OnKeyDown();
};

#endif