#ifndef _dicomimageinteractorstyle_h
#define _dicomimageinteractorstyle_h
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageViewer2.h>
#include <vtkObjectFactory.h>
#include <vtkImageViewer2.h>
#include <vtkPropPicker.h>
#include <vtkImageAlgorithm.h>
#include "ItkTypedefs.h"

class DicomImageInteractorStyle : public vtkInteractorStyleImage{
public:
	static DicomImageInteractorStyle* New();
	vtkTypeMacro(DicomImageInteractorStyle, vtkInteractorStyleImage);
	void SetImageViewer(vtkSmartPointer<vtkImageViewer2> iv);
	void SetSegmentador(TSegmentator::Pointer s){Segmentador = s;}
	void SetImageInput(vtkSmartPointer<vtkImageAlgorithm> a){VtkImage = a;}
protected:
	vtkSmartPointer<vtkImageAlgorithm> VtkImage;
	int* coords;
	short val;
	int MinSlice;
	int MaxSlice;
	int Slice;
	vtkSmartPointer<vtkImageViewer2> ImageViewer;
	TSegmentator::Pointer Segmentador;
	virtual void OnKeyDown();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();

	void MoveForward();
	void MoveBackward();
	void OnMouseMove();
	void OnLeftButtonDown();
	void OnLeftButtonUp();
	DicomImageInteractorStyle(){
		Picker = vtkSmartPointer<vtkPropPicker>::New();
		coords = new int[3];
	}
	~DicomImageInteractorStyle(){
		delete coords;
	}
private:
	int CurrentCellId;
	int* CurrentCellCoordinate;
	vtkSmartPointer<vtkPropPicker> Picker;
};

#endif