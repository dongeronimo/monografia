#include "DicomImageInteractorStyle.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <vtkProp3D.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
vtkStandardNewMacro(DicomImageInteractorStyle);
//-----------------------------------------------------------
void DicomImageInteractorStyle::SetImageViewer(vtkSmartPointer<vtkImageViewer2> iv){
	this->ImageViewer = iv;
	this->MinSlice = iv->GetSliceMin();
	this->MaxSlice = iv->GetSliceMax();
	this->Slice = this->MinSlice;
	vtkDebugMacro("Fatias: ["<<this->MinSlice<<","<<this-MaxSlice<<"]");
	
}

void DicomImageInteractorStyle::MoveForward(){
	if(this->Slice < this->MaxSlice){
		/* Não funciona - o windowLevel é null.
		vtkSmartPointer<vtkImageMapToWindowLevelColors> windowLevel = this->ImageViewer->GetWindowLevel();
		
		vtkScalarsToColors* colors = windowLevel->GetLookupTable();
		vtkColorTransferFunction* foo = vtkColorTransferFunction::SafeDownCast(colors);
		vtkLookupTable* bar = vtkLookupTable::SafeDownCast(colors);
		*/
		this->Slice = this->Slice+1;
		vtkDebugMacro("slice = "<<this->Slice);
		this->ImageViewer->SetSlice(this->Slice);
		this->ImageViewer->Render();
	}
}

void DicomImageInteractorStyle::MoveBackward(){
	if(this->Slice > this->MinSlice){
		this->Slice = this->Slice-1;
		vtkDebugMacro("slice = "<<this->Slice);
		this->ImageViewer->SetSlice(this->Slice);
		this->ImageViewer->Render();
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void DicomImageInteractorStyle::OnMouseMove(){
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkCellPicker> _picker = vtkSmartPointer<vtkCellPicker>::New();
	vtkRenderer* ren = this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	_picker->SetTolerance(0.0005);
	_picker->Pick(pos[0], pos[1], 0, ren);
	double* worldPosition = _picker->GetPickPosition();
	cout<<"cell id is: "<<_picker->GetCellId()<<endl;
	int* coords = _picker->GetCellIJK();
	cout<<"cell coords is: "<<coords[0]<<","<<coords[1]<<","<<coords[2]<<endl;//aqui eu tenho qual cell.
	//this->
}

void DicomImageInteractorStyle::OnKeyDown(){
	std::string tecla = this->GetInteractor()->GetKeySym();
	vtkDebugMacro("tecla: "<<tecla);
	if(tecla=="Up"){
		MoveForward();
	}
	else if (tecla=="Down"){
		MoveBackward();
	}else if(tecla=="p"){
	}
}

void DicomImageInteractorStyle::OnMouseWheelForward(){
	MoveForward();
}

void DicomImageInteractorStyle::OnMouseWheelBackward(){
	MoveBackward();
}