#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "ppmloader/ppmloader.h"
#include "matriz.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <limits>
#include <algorithm>
#include <getopt.h>

using namespace std;
/*
struct info_archivo
{
	std::string inPath;
	std::string outPath;
	int cantidadRayos;
};
*/

class IOUtils {
public:
	IOUtils(int argc, char** argv);
	~IOUtils();
	Matriz<double> leer_imagen();
	void escribir_imagen(Matriz<double> imagen);
	
	//info_archivo info;
	std::string inPath;
	std::string outPath;
	int cantidadRayos;
	int discretizadoFilas = 50;
	int discretizadoColumnas = 50;
};

//IOUtils::IOUtils(){
//}

IOUtils::~IOUtils(){
}


IOUtils::IOUtils(int argc, char** argv){

    if (argc < 4)
    {
        std::cout<<"Parametros de entrada insuficientes"<<'\n';
    }
	cantidadRayos = std::atoi(argv[1]);
	inPath = argv[2];
	outPath = argv[3];
}   

Matriz<double> IOUtils::leer_imagen()
{
    uchar* imagen;
	int alto, ancho;
    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
    LoadPPMFile(&imagen, &ancho, &alto, &pt, inPath.c_str());

    //Pixeles en RGB de una imagen en escala de grises, 
	// R = G = B = intensidad de gris
	
    Matriz<double> temp(alto, ancho);

    for(unsigned int i = 0;i < alto; i++)
    {
		for(unsigned int j; j < ancho; j++)
		{
        	temp[i][j] = double(imagen[j*3]);
		}
    }

    delete [] imagen;    
    return temp;
}


void IOUtils::escribir_imagen(Matriz<double> imagen)
{
	char comments[10];
	sprintf(comments, "%s", "TP3");
	int col = discretizadoColumnas;
	int fil = discretizadoFilas;
	uchar* data = new uchar[fil*col*3];
	for(int i = 0; i < fil; i++)
	{
		for(int j = 0; j < col*3; j++)
		{
			data[(i*col)+j] = data[(i*col)+j+1] = data[(i*col)+j+2] = uchar(imagen[i][j]);
		}
	}
	bool ret = SavePPMFile(outPath.c_str(),data,col,fil,PPM_LOADER_PIXEL_TYPE_RGB_8B, comments);
	if (!ret)
	{
		std::cout << "ERROR: couldn't save Image to ppm file" << std::endl;
	}
}
#endif
