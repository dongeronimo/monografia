#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include "PontoNaTela.h"
#include <vector>
#include <vtkRenderer.h>
#include <vtkPoints.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkCellArray.h>
#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkTriangleFilter.h>
using namespace std;

#pragma once
//S�lido gerado a partir do contorno desenhado na tela.
//O objetivo � gerar um vtkPolyData a partir do contorno extrudado ao longo do vetor da dire��o 
////de olhar da c�mera pra dentro da tela.
//A classe recebe a lista de pontos e outros params necess�rios, gera a superficie e a extruda.
//O s�lido (o vtkPolyData) estar� disponivel em GetPolyData().
class SolidoDeEstrusao
{
private:
	vtkSmartPointer<vtkPoints> Pontos;
	vtkSmartPointer<vtkPolyData> Contorno;
	vtkSmartPointer<vtkLinearExtrusionFilter> Extrusor;
	vtkSmartPointer<vtkPolyData> Resultado;
	vtkSmartPointer<vtkPolyDataMapper> MapperDoContorno;
	vtkSmartPointer<vtkActor> PropDoContorno;
	vtkSmartPointer<vtkTriangleFilter> Triangulador;
	void FabricarExtrusao(double vetor[3]);
	void FabricarRepresentacao();
	void FabricarTriangulador();
	vtkSmartPointer<vtkPolyData> FabricarContorno(vtkSmartPointer<vtkPoints> pts);
public:
	vtkSmartPointer<vtkActor> GetPropDoContorno();
	vtkSmartPointer<vtkPolyData> GetPolyData();
	SolidoDeEstrusao(vtkSmartPointer<vtkPoints> lstPontos, double vetor[3], double size);
	SolidoDeEstrusao(vector<PontoNaTela> lstPontos, vtkSmartPointer<vtkRenderer> renderer);
	~SolidoDeEstrusao(void);
};

