#include "matriz_aux.hpp"
#include <cassert>
Matriz<double> resolverEN(Matriz<double> A, Matriz<double> b)
{
	//assert(A.rango() < A.columnas()); //como es SDP es inversible 
	//
	Matriz<double> At = A.traspuesta();
	Matriz<double> Ata = At*(A);
	Matriz<double> Atb = At*(b);
	Matriz<double> x = resolver_gauss(Ata, Atb);
	return x;
}
