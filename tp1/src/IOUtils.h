#ifndef IOUTILS_H_
#define IOUTILS_H_
//#include "Matriz.h"
#include <getopt.h>
#include "PageRank.h"
//#include <string>

//#define MEDIR_TIEMPO_INICIO(timer) timer = clock();
//#define MEDIR_TIEMPO_FIN(timer) timer = clock() - timer;

struct config
{
	bool timer_flag = false;   // determina si se calculará el tiempo de ejecución
    double timer = 0.0;         // timer

    string input_file;
    string output_file;
    double p = 0.0;
};

class IOUtils {
public:
	IOUtils();
	~IOUtils();
	config parse(int argc, char** argv);
	PageRank leerEntrada(const std::string &nombreArchivo);
	void escribirSalida(const std::string &nombreArchivo, double p, const std::vector<double>& rank);
}; 
#endif
