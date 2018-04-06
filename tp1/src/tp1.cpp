#include "IOUtils.h"
#include <chrono>

using namespace std;

static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}


int main(int argc, char* argv[])
{

	IOUtils utilidad;
	config args = utilidad.parse(argc,argv);
	PageRank rank = utilidad.leerEntrada(args.input_file);
	
	start_timer();
	rank.calcularRanking(args.p);
	args.timer = stop_timer();
	utilidad.escribirSalida(args.output_file,args.p,rank.GetRanking());

	if(args.timer_flag) cout << args.timer << endl;

	return 0;
}