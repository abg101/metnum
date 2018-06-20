#include "matriz.hpp"
#include <cassert>
Matriz<int> resolverEN(Matriz<int> A, vector<int> b)
{
	//assert(A.rango() < A.columnas()); //TODO: implementar rango o asumirlo completo 
	Matriz<int> At = A.traspuesta();
	Matriz<int> lower = cholesky(At*A);
	Matriz<int> x = (L*L.traspuesta())/(At*b);
	return x;
}
