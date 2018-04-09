#include "IOUtils.h"
#include <iostream>
#include <fstream>


using namespace std;

string GetFilename (const string& str)
{
  size_t ultimo = str.find_last_of("/\\");
  return str.substr(ultimo+1);
}

string GetPath (const string& str)
{
  size_t ultimo = str.find_last_of("/\\");
  return str.substr(0,ultimo+1);
}

IOUtils::IOUtils(){
}

IOUtils::~IOUtils(){
}

config IOUtils::parse(int argc, char** argv){

	config args;

	args.input_file = argv[1];
	args.output_path = "";//GetPath(args.input_file);
	args.output_file = GetFilename(args.input_file)+".out";
	args.p = stod(argv[2]);

	char option;
    while ((option = getopt(argc, argv, "te:o:")) != -1) {
        switch (option) {
            case 't': { // calcular tiempo de ejecución
                args.timer_flag = true;
                break;
            }
            case 'e':{	
            	args.output_path = optarg;
            	break;
            }
            case 'o':{
            	args.output_file = optarg;
            	break;
            }         

            default: { // si las opciones son inválidas
            
                exit(1);
                break;
            }
        }

    }
    return args;

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

void IOUtils::escribirSalida(const vector<double>& rank, const config &args){
	
	ofstream out(args.output_path+args.output_file);

	if(!out){
		cerr << "No se puede crear el archivo: " << args.output_path+args.output_file<< endl;
		exit(4);
	}
	out << args.p << endl;
	for(std::vector<double>::const_iterator it=rank.begin(); it!=rank.end(); ++it){
		out << *it << endl;
	}
	out.flush();
	out.close();

	if(args.timer_flag) cout << (args.timer) << endl;
} 



