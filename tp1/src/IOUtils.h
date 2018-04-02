#ifndef IOUTILS_H_
#define IOUTILS_H_
//#include "Matriz.h"
#include "PageRank.h"

class IOUtils {
public:
	IOUtils();
	~IOUtils();
	PageRank leerEntrada(const std::string &nombreArchivo);		
	void escribirSalida(const std::string &nombreArchivo, double p, const std::vector<double>& rank);
}; 
#endif
