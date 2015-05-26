#include "SolidoDeExtrusao.h"
#include <vector>
#include <vtkProperty.h>
using namespace std;

void SolidoDeEstrusao::FabricarTriangulador()
{
	Triangulador = vtkSmartPointer<vtkTriangleFilter>::New();
	Triangulador->SetInputData(Resultado);
	Triangulador->Update();
}

vtkSmartPointer<vtkPolyData> SolidoDeEstrusao::GetPolyData()
{
	return Triangulador->GetOutput();
}

vtkSmartPointer<vtkActor> SolidoDeEstrusao::GetPropDoContorno()
{
	return PropDoContorno;
}

vtkSmartPointer<vtkPolyData> SolidoDeEstrusao::FabricarContorno(vtkSmartPointer<vtkPoints> pts)
{
	vtkSmartPointer<vtkCellArray> poly = vtkSmartPointer<vtkCellArray>::New();
	int cellSize = Pontos->GetNumberOfPoints();
	poly->InsertNextCell(cellSize);
	for(int i=0; i<cellSize; i++){
		poly->InsertCellPoint(i);
	}
	vtkSmartPointer<vtkPolyData> pdata = vtkSmartPointer<vtkPolyData>::New();
	pdata->SetPoints(Pontos);
	pdata->SetPolys(poly);
	return pdata;
}

SolidoDeEstrusao::SolidoDeEstrusao(vtkSmartPointer<vtkPoints> lstPontos, double vetor[3], double size)
{
	//a lista já está criada
	Pontos = vtkSmartPointer<vtkPoints>::New();
	Pontos->DeepCopy(lstPontos);
	Contorno = FabricarContorno(Pontos);
	
	FabricarExtrusao(vetor);

	FabricarRepresentacao();

	FabricarTriangulador();
}

void SolidoDeEstrusao::FabricarExtrusao(double vetor[3])
{
	Extrusor = vtkSmartPointer<vtkLinearExtrusionFilter>::New();
	Extrusor->CappingOn();
	Extrusor->SetInputData(Contorno);
	Extrusor->SetScaleFactor(5000);
	Extrusor->SetExtrusionTypeToVectorExtrusion();
	Extrusor->SetVector(vetor);
	Extrusor->Update();
	Resultado = Extrusor->GetOutput();
}

void SolidoDeEstrusao::FabricarRepresentacao()
{
	MapperDoContorno = vtkSmartPointer<vtkPolyDataMapper>::New();
	MapperDoContorno->SetInputData(Contorno);
	PropDoContorno = vtkSmartPointer<vtkActor>::New();
	PropDoContorno->GetProperty()->SetRepresentationToWireframe();
	PropDoContorno->SetMapper(MapperDoContorno);
}

SolidoDeEstrusao::SolidoDeEstrusao(vector<PontoNaTela> lstPontos, vtkSmartPointer<vtkRenderer> renderer)
{
	//cria a lista de pontos.
	this->Pontos = vtkSmartPointer<vtkPoints>::New();
	//bacalhau pq não sei trabalhar direito com vtkCoordinate.
	vector<vtkSmartPointer<vtkCoordinate>> lstCoordinates;
	for(unsigned int i=0; i<lstPontos.size(); i++){
		vtkSmartPointer<vtkCoordinate> c = vtkSmartPointer<vtkCoordinate>::New();
		c->SetCoordinateSystemToDisplay();
		lstCoordinates.push_back(c);
	}
	for(unsigned int i=0; i<lstPontos.size(); i++){
		PontoNaTela p = lstPontos[i];
		vtkCoordinate* coord = lstCoordinates[i];
		coord->SetValue(p.x, p.y);
		double* wc = coord->GetComputedWorldValue(renderer);//é aqui que uso o renderer pedido no construtor
		cout<<"ponto = "<<wc[0]<<" , "<<wc[1]<<" , "<<wc[2]<<endl;
		Pontos->InsertNextPoint(wc[0], wc[1], wc[2]);
	}
	//cria o contorno
	Contorno = FabricarContorno(Pontos);
	//cria a extrusão
	double* v = renderer->GetActiveCamera()->GetDirectionOfProjection();
	cout<<"direction of projection :"<<v[0]<<","<<v[1]<<","<<v[2]<<endl;
	double vetor[] = {v[0], v[1], v[2]};
	
	FabricarExtrusao(vetor);

	FabricarRepresentacao();

	FabricarTriangulador();
}


SolidoDeEstrusao::~SolidoDeEstrusao(void)
{
}
