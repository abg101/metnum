#include "matriz_aux.hpp"
#include <cassert>
Matriz<double> resolverEN(Matriz<double> A, Matriz<double> b)
{
	//assert(A.rango() < A.columnas()); //como es SDP es inversible 
	Matriz<double> At = A.traspuesta();
	Matriz<double> x = resolver_gauss(At*(A),At*(b));
	return x;
}
