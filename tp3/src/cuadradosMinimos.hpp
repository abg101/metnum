#include "matriz.hpp"
#include <cassert>
Matriz<int> resolverEN(Matriz<int> A, vector<int> b)
{
	//assert(A.rango() < A.columnas()); //TODO: implementar rango o asumirlo completo 
	Matriz<int> At = A.traspuesta();
	int n = A.columnas();
	Matriz<int> x = Matriz(n,1);
	
	x = (A*L.traspuesta())/(At*b);
	return x;
}
