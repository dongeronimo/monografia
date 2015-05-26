#include <iostream>
#include "VolumeData.h"
#include "ItkTypedefs.h"
#include <vtkImageImport.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageAlgorithm.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkImageData.h>
#include "VolumeInteractorStyle.h"
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include "DicomImageInteractorStyle.h"
#include <vtkDICOMImageReader.h>

TImageSource::Pointer CreateImageSource(VolumeData d);
vtkImageAlgorithm* CreateVTKImage(VolumeData d);
vtkGPUVolumeRayCastMapper* CreateMapper(vtkImageAlgorithm* data);
vtkVolume* CreateActor(vtkVolumeMapper* mapper);
TMaskSource::Pointer CreateMask(VolumeData d);


int main(int argc, char** argv){
	//pega o diretório onde a série está.
	std::string diretorio(argv[1]);
	VolumeData dados = LoadFromFile(diretorio.c_str());

	TImageSource::Pointer ImporterToItk = CreateImageSource(dados);
	
	TStencilFilter::Pointer Stencil = TStencilFilter::New();
	Stencil->InPlaceOn();
	Stencil->SetInput(ImporterToItk->GetOutput());
	Stencil->Update();

	vtkImageAlgorithm* ImporterToVtk = CreateVTKImage(dados);
	vtkGPUVolumeRayCastMapper* Mapper = CreateMapper(ImporterToVtk);
	vtkVolume* Actor = CreateActor(Mapper);

	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->GetActiveCamera()->ParallelProjectionOn();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkSmartPointer<VolumeInteractionStyle> VolumeInteraction = vtkSmartPointer<VolumeInteractionStyle>::New();
	VolumeInteraction->SetImageData(ImporterToVtk->GetOutput());
	VolumeInteraction->SetStencilFilter(Stencil);
	VolumeInteraction->SetMapper(Mapper);
	VolumeInteraction->SetVTKImporter(ImporterToVtk);
	iren->SetInteractorStyle(VolumeInteraction);
	iren->SetRenderWindow(renWin);
 
	ren1->AddActor(Actor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	renWin->SetSize(200, 200);
 
	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);
	renWin->Render();
	
	vtkSmartPointer<vtkRenderWindow> DicomWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetRenderWindow(DicomWindow);//adiciona o visualizador de série à janela principal
	imageViewer->SetInputConnection(ImporterToVtk->GetOutputPort());//conecta o visualizador de dicoms à saída da pipeline
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<DicomImageInteractorStyle> dicomStyle = vtkSmartPointer<DicomImageInteractorStyle>::New();
	dicomStyle->SetImageViewer(imageViewer);
	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(dicomStyle);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();	

	iren->Start();
	return 0;
}

TMaskSource::Pointer CreateMask(VolumeData d){
	typedef itk::ImportImageFilter<bool, 3> TMaskCreator;	
	TMaskCreator::Pointer creator = TMaskCreator::New();
	TMaskCreator::SizeType size;
	size[0] = d.SizeX;
	size[1] = d.SizeY;
	size[2] = d.SizeZ;
	TMaskCreator::IndexType start;
	start.Fill( 0 );
	TMaskCreator::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );

	creator->SetRegion(region);
	double origin[] = {d.CenterX,d.CenterY, d.CenterZ};
	creator->SetOrigin(origin);
	double spacing[] = {d.SpacingX,d.SpacingY,d.SpacingZ};
	creator->SetSpacing(spacing);
	int dimensoesTotais = d.SizeX * d.SizeY * d.SizeZ;
	bool* buffer = (bool*)malloc(dimensoesTotais*sizeof(bool));
	creator->SetImportPointer(buffer, dimensoesTotais, true);
	creator->Update();
	return creator;
}

vtkVolume* CreateActor(vtkVolumeMapper* mapper){
	vtkSmartPointer<vtkVolume> vol = vtkVolume::New();
	
	vtkSmartPointer<vtkPiecewiseFunction> ScalarOpacity = vtkPiecewiseFunction::New();
	ScalarOpacity->AddSegment(100,0,300,1);
	
	vtkSmartPointer<vtkColorTransferFunction> ColorFn = vtkColorTransferFunction::New();
	ColorFn->AddRGBSegment(0, 1,1,1, 300, 1,1,1);

	vtkVolumeProperty* properties = vtkVolumeProperty::New();
	properties->ShadeOn();
	properties->SetInterpolationTypeToLinear();
	properties->SetScalarOpacity(ScalarOpacity);
	properties->SetColor(ColorFn);
	vol->SetProperty(properties);
	vol->SetMapper(mapper);
	vol->Update();
	return vol;
}

vtkGPUVolumeRayCastMapper* CreateMapper(vtkImageAlgorithm* data){
	vtkGPUVolumeRayCastMapper* Mapper = vtkGPUVolumeRayCastMapper::New();
	vtkImageData* d = data->GetOutput();
	Mapper->SetInputData(d);
	Mapper->Update();
	return Mapper;
}

vtkImageAlgorithm* CreateVTKImage(VolumeData d){
	vtkSmartPointer<vtkImageImport> Importer = vtkImageImport::New();
	Importer->SetWholeExtent(0, d.SizeX-1, 0,d.SizeY-1, 0,d.SizeZ-1);
	Importer->SetDataExtentToWholeExtent();
	Importer->SetDataSpacing(d.SpacingX,d.SpacingY,d.SpacingZ);
	Importer->SetDataOrigin(d.CenterX, d.CenterY, d.CenterZ);
	Importer->SetDataScalarTypeToShort();
	Importer->SetImportVoidPointer(d.Data);
	Importer->Update();
	return Importer;
}

TImageSource::Pointer CreateImageSource(VolumeData d){
	TItkImageImport::SizeType size;
	size[0] = d.SizeX;
	size[1] = d.SizeY;
	size[2] = d.SizeZ;
	TItkImageImport::IndexType start;
	start.Fill( 0 );
	TItkImageImport::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );
	TItkImageImport::Pointer Importer = TItkImageImport::New();
	Importer->SetRegion(region);
	double origin[] = {d.CenterX,d.CenterY, d.CenterZ};
	Importer->SetOrigin(origin);
	double spacing[] = {d.SpacingX,d.SpacingY,d.SpacingZ};
	Importer->SetSpacing(spacing);
	int dimensoesTotais = d.SizeX * d.SizeY * d.SizeZ;
	Importer->SetImportPointer(d.Data, dimensoesTotais, false);
	Importer->Update();
	return Importer;
}