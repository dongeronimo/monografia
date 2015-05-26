#ifndef __MyItkStencil_hxx
#define __MyItkStencil_hxx
#include "MyItkStencil.h"
#include <thread>

using namespace std;
namespace itk{

template<class TImage> MyItkStencil<TImage>::MyItkStencil()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	NumberOfCores = sysinfo.dwNumberOfProcessors;
	this->StencilValue = -25000;
	
}

template<class TImage> void MyItkStencil<TImage>::AddStencil(vtkSmartPointer<vtkImageStencilData> stencil){
	Stencils.push_back(stencil);
	Modified();
}
//-----------------------------------------------------------------------------------------------
template<class TImage> void MyItkStencil<TImage>::GenerateData(){
	typename TImage::ConstPointer input = this->GetInput();
	typename TImage::Pointer output = this->GetOutput();
	long t0 = 0; long t1 = 0;
	// This call must come before the GetRunningInPlace() call or the
	// GetRunningInPlace() will return an incorrect result.
	this->AllocateOutputs();
	if (!this->GetRunningInPlace()){
		std::cerr << "The idea of this example is to run inplace, something is wrong!" << std::endl;
		return;
	}
	//combina os stencils

	t0 = GetCurrentTime();
	if(this->Stencils.size()>0){
		vtkSmartPointer<vtkImageStencilData> StencilResultante = Stencils[0];
		for(unsigned int i=1; i<Stencils.size(); i++){
			StencilResultante->Add(Stencils[i]);
		}
		t1 = GetCurrentTime();
		cout<<"Custo da soma de stencils:"<<(t1-t0)<<endl;
		//o stencil final está pronto, em StencilResultante.
		//a iteração propriamente dita
		t0 = GetCurrentTime();
		//1a experiência - iteração na marra;  UGLY HACK. Devia ser feito usando os iterator dos stencils
		//mas eu ainda não sei usá-los.
		int szX = output->GetLargestPossibleRegion().GetSize()[0];
		int szY = output->GetLargestPossibleRegion().GetSize()[1];
		int szZ = output->GetLargestPossibleRegion().GetSize()[2];
		int numeroDeElementos = output->GetLargestPossibleRegion().GetNumberOfPixels();
		
		//UGLY HACK - Devia usar um tipo parametrizado pra ficar com cara de algoritmo da itk
		short* ptr = output->GetBufferPointer();
		std::vector<thread> workers;
		int szParticao = numeroDeElementos / NumberOfCores;
		for(int i = 0; i<NumberOfCores; i++){
			int inicio = szParticao * i;
			int fim = inicio+szParticao;
			if(i==NumberOfCores-1){
				fim = fim + szParticao % NumberOfCores; // PEGA OS RESTOS DA DIVISÃO
			}
			short val = this->StencilValue;
			workers.push_back(thread([szParticao, ptr, StencilResultante, szX, szY, szZ, inicio, fim, val](){
				for(int i=inicio; i<fim; i++){
					int x = i % szX;
					int y = (i % (szX*szY)) / szX;
					int z = i / (szX*szY);
					if(StencilResultante->IsInside(x,y,z))
					{
						ptr[i] = val;//muda o valor se estiver
					}

				}
			}));
		}//fim do for criador dos workers
		for(auto& thread:workers){
			thread.join();
		}
		long t = GetCurrentTime();
		cout<<"Custo do corte:"<<t-t0<<endl;	

	}
}
}//do namespace
#endif