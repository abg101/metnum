//include "Matriz.h"
//#include <iostream>
#include "IOUtils.h"

using namespace std;


int main(int argc, char const *argv[])
{
	string archivo_entrada = argv[1];
	double p = stod(argv[2]);

	IOUtils utilidad;
	PageRank rank = utilidad.leerEntrada("../tests/catedra/tests_tp1/"+archivo_entrada);


	rank.calcularRanking(p);

	utilidad.escribirSalida("archivo_entrada.txt",p,rank.GetRanking());


	return 0;
}