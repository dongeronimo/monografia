#ifndef __serie_h
#define __serie_h
#include <string>
//A estrutura com os dados do volume.
struct VolumeData{
	short* Data;
	int SizeX,SizeY,SizeZ;
	double SpacingX, SpacingY, SpacingZ;
	double CenterX, CenterY, CenterZ;
};
//Carrega o volume
VolumeData LoadFromFile(std::string dir);

#endif