#include "IOUtils.h"
#include <ctime>

using namespace std;

time_t start_time;


void start_timer() {
    start_time = clock();
}

double stop_timer() {
    auto end_time = clock();
    return (end_time - start_time /(double)  CLOCKS_PER_SEC);
}

int main(int argc, char* argv[])
{

	IOUtils utilidad;
	config args = utilidad.parse(argc,argv);
	PageRank rank = utilidad.leerEntrada(args.input_file);

    start_timer();
	rank.calcularRanking(args.p);
	args.timer = stop_timer();

	utilidad.escribirSalida(rank.GetRanking(),args);

	return 0;
}


