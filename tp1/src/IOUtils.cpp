#include "IOUtils.h"
#include "Matriz.h"
#include <iostream>
#include <fstream>


using namespace std;

IOUtils::IOUtils(){
}

IOUtils::~IOUtils(){
}

Matriz IOUtils::leerEntrada(const string &nombreArchivo){
	ifstream in(nombreArchivo.c_str());
	if(!in){
		cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
		exit(1);
	}
	unsigned long num_pag=0, num_links=0, pag_i=0, pag_j=0;
	in >> num_pag >> num_links;
	if(num_pag==0 || num_links==0){
		cerr << "La cantidad de paginas o links debe ser mayor que cero" << endl;
		exit(1);
	}
	Matriz mat(num_pag, num_pag);
	for(unsigned long i=0; i<num_links; i++){
		in >> pag_i >> pag_j;
		if(!in.good()){
			cerr << "Error archivo: " << nombreArchivo << " incompleto, se esperaban: " 
                             << num_links << " links y se leyeron hasta: " << i << " links." << endl;
			exit(2);
		}
		if(pag_i<1 || pag_i>num_pag || pag_j<1 || pag_j>num_pag){
			cerr << "Link de pag: " << pag_i << " a " << pag_j
			     << " fuera de rango, las paginas van de: " << 1 
                             << " hasta " << num_pag << "." << endl;
			exit(3);          
		}
		mat.Set(1, pag_i, pag_j);
	}
	in.close();
	return mat;
}

void IOUtils::escribirSalida(const string &nombreArchivo, double p, const std::vector<double> rank){
	ofstream out(nombreArchivo.c_str());
	if(!out){
		cerr << "No se puede crear el archivo: " << nombreArchivo << endl;
		exit(4);
	}
	out << p << endl;
	for(std::vector<double>::const_iterator it=rank.begin(); it!=rank.end(); ++it){
		out << *it << endl;
	}
	out.flush();
	out.close();
} 

