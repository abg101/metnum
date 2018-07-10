#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <limits>
#include "matriz_aux.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>// max y transform
#include "IOUtils.hpp"
#include "cuadradosMinimos.hpp"
#include "generadorRayos.hpp"
#include "ruido.hpp"
using namespace std;

int main(int argc, char* argv[]){
	//Paso 1: Leer imagen
	IOUtils io(argc,argv);
	Matriz<double> imagen = io.leer_imagen();

	//Paso 2: Simular el proceso tomografico
	auto tiemposRayos = generarRayos(imagen, io.discretizadoFilas, io.discretizadoColumnas, io.cantidadRayos); 

	Matriz<double> tiempos = std::get<0>(tiemposRayos);
	Matriz<double> D = std::get<1>(tiemposRayos);

	//Paso 3: Ruido aleatorio
	
	Matriz<double> tiempoConRuido = generarRuido(tiempos);

	//Paso 4: Reconstruir el cuerpo original
	
	cout << "Tiempos:"<<tiempos;
	cout << "Tiempo con ruido:"<<tiempoConRuido;
	//cout << "D :" << D;

	Matriz<double> reconstruccion = resolverEN(D, tiempos);//TODO volver a tiemposConRuido
	
	cout << "Reconstruccion: \n" << reconstruccion;

	io.escribir_imagen(reconstruccion);
	
}
