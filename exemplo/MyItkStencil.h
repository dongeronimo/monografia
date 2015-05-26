#ifndef __MyItkStencil_h
#define __MyItkStencil_h
#include <itkInPlaceImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkImageStencilData.h>
#include <Windows.h>
#include <vector>
namespace itk{

template <class TImage> class MyItkStencil:public InPlaceImageFilter<TImage>{
public:
	//standar class typedefs
	typedef MyItkStencil Self;
	typedef InPlaceImageFilter<TImage> Superclass;
	typedef SmartPointer<Self> Pointer;
	//para criação através da fábrica de objetos
	itkNewMacro(Self);
	//rtti
	itkTypeMacro(MyItkStencil, InPlaceImageFilter);
	void SetStencilValue(short val){
		this->StencilValue = val;
	};
	void AddStencil(vtkSmartPointer<vtkImageStencilData> stencil);
protected:
	MyItkStencil();
	~MyItkStencil(){}
	virtual void GenerateData();
	std::vector<vtkSmartPointer<vtkImageStencilData>> Stencils;
private:
	short StencilValue;
	int NumberOfCores;
	MyItkStencil(const Self&);
	void operator=(const Self&);
};
}
#ifndef ITK_MANUAL_INSTANTIATION
#include "MyItkStencil.cpp"
#endif
#endif