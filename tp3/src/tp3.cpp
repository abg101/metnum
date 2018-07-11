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
/*	IOUtils io(argc,argv);
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
*/
	Matriz<double> A(3,3,0);
     Matriz<double> b(3,1,0);
     
     Matriz<double> temp1(3, 1, 0);
    Matriz<double> temp2(3, 1, 0);
    Matriz<double> temp3(3, 1, 0); 
     
    temp1[0][0] = 1;
    temp1[1][0] = -2;
    temp1[2][0] = 0.5;
    temp2[0][0] = -2;
    temp2[1][0] = 5;
    temp2[2][0] = -1.5;
    temp3[0][0] = -0.2;
    temp3[1][0] = 1.75;
    temp3[2][0] = -1;

    A.set_fil(0,temp1);
    A.set_fil(1,temp2);
    A.set_fil(2,temp3);
    b[0][0]=-5;
    b[1][0]=0;
    b[2][0]=10;
		
	cout << "|" << A[0][0] << " " << A[0][1]<< " " << A[0][2] << " |"<< b[0][0]<< "|" << endl;
    cout << "|" << A[1][0] << " " << A[1][1] << " "<< A[1][2] << " |" << b[1][0]<< "|" << endl;
    cout << "|" << A[2][0] << " " << A[2][1] << " "<< A[2][2] << " |" << b[2][0]<< "|" << endl;

    Matriz<double> res = resolver_gauss(A,b);

    cout << "res:       " << endl;
    cout << "|" << res[0][0] << "|" << endl;
    cout << "|" << res[1][0] << "|" << endl;
    cout << "|" << res[2][0] << "|" << endl;
//    http://www1.eafit.edu.co/cursonumerico/capitulo3/sesion_1/documentos/eliminacion_gausiana.pdf ,pagina 6
return 0;	
}
