#include "VolumeInteractorStyle.h"
vtkStandardNewMacro(VolumeInteractionStyle);
void VolumeInteractionStyle::OnKeyDown(){
	vtkRenderer* ren = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	string key = Interactor->GetKeySym();
	cout<<key<<endl;
	if(key=="a"||key=="A"){
		int screenX = this->Interactor->GetEventPosition()[0];
		int screenY = this->Interactor->GetEventPosition()[1];
		PontoNaTela p(screenX, screenY);
		Pontos.push_back(p);
		cout<<screenX<<";"<<screenY<<" adicionado"<<endl;
	}
	else if(key=="Return"){
		assert(Image!=0);
		//S� de construir o s�lido eu j� tenho o contorno e o polydata do s�lido
		SolidoDeEstrusao solido(Pontos, ren);
		//ritual pra gerar o stencil a partir de pol�gonos
		vtkSmartPointer<vtkPolyData> Regi�oDeRemo��o = solido.GetPolyData();
		vtkSmartPointer<vtkPolyDataToImageStencil> PolydataToStencil = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
		PolydataToStencil->SetInputData(Regi�oDeRemo��o);
		PolydataToStencil->SetInformationInput(Image);
		PolydataToStencil->Update();
		vtkSmartPointer<vtkImageStencilData> GeneratedStencilData = PolydataToStencil->GetOutput();//aqui eu tenho o stencil, pronto pra ser usado
		//usa-o

		//bota o contorno na tela
		vtkSmartPointer<vtkActor> atorDoSolido = solido.GetPropDoContorno();
		Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddViewProp(atorDoSolido);
		Pontos.clear();
		//atualiza a tela.
		ren->Render();
	}
}