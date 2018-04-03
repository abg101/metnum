#include "IOUtils.h"
//#include "Matriz.h"
#include <iostream>
#include <fstream>


using namespace std;

IOUtils::IOUtils(){
}

IOUtils::~IOUtils(){
}

PageRank IOUtils::leerEntrada(const string &nombreArchivo){
	ifstream in(nombreArchivo.c_str());
	if(!in){
		cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
		exit(1);
	}
	unsigned int num_pag=0, num_links=0, pag_i=0, pag_j=0;
	if(!(in >> num_pag >> num_links)){
		cerr << "No se pudo leer el numero de paginas o el numero de links." << endl;
		exit(1);
	}
	if(num_pag==0){
		cerr << "La cantidad de paginas debe ser mayor que cero" << endl;
		exit(1);
	}
	Matriz mat(num_pag, num_pag);
	vector<int> gradoDePaginas(num_pag);
	for(unsigned int i=0; i<num_links; i++){
		if(!(in >> pag_i >> pag_j)){
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
		mat.Set(1, pag_j, pag_i);
		gradoDePaginas[pag_i-1]++;

	}
	in.close();
	PageRank pageRank(mat,gradoDePaginas);
	return pageRank;
}

void IOUtils::escribirSalida(const string &nombreArchivo, double p, const std::vector<double>& rank){
	ofstream out(nombreArchivo.c_str());
	if(!out){
		cerr << "No se puede crear el archivo: " << nombreArchivo << endl;
		exit(4);
	}
	out << p << endl;
	cout << "despues de p" << endl;
	for(std::vector<double>::const_iterator it=rank.begin(); it!=rank.end(); ++it){
		out << *it << endl;
	}
	cout << "depsues de imprimir" << endl;
	out.flush();
	out.close();
} 

