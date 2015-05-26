#ifndef _dicomimageinteractorstyle_h
#define _dicomimageinteractorstyle_h
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageViewer2.h>
#include <vtkObjectFactory.h>
#include <vtkImageViewer2.h>
#include <vtkPropPicker.h>

class DicomImageInteractorStyle : public vtkInteractorStyleImage{
public:
	static DicomImageInteractorStyle* New();
	vtkTypeMacro(DicomImageInteractorStyle, vtkInteractorStyleImage);
	void SetImageViewer(vtkSmartPointer<vtkImageViewer2> iv);
protected:
	int MinSlice;
	int MaxSlice;
	int Slice;
	vtkSmartPointer<vtkImageViewer2> ImageViewer;
	virtual void OnKeyDown();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseMove();
	void MoveForward();
	void MoveBackward();

	DicomImageInteractorStyle(){
		Picker = vtkSmartPointer<vtkPropPicker>::New();
	}
private:
	int CurrentCellId;
	int* CurrentCellCoordinate;
	vtkSmartPointer<vtkPropPicker> Picker;
};

#endif