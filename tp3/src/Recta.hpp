#ifndef RECTA_HPP
#define RECTA_HPP
#include <cassert>
class Recta
{
	public:
		Recta(int x1, int y1, int x2, int y2);
		int evaluar(int x);
	private:
		int coeficiente;
		int independiente;
};

Recta::Recta(int x1, int y1, int x2, int y2)
{
	if(x1 == x2)
	{//hack
		coeficiente = 1;
		independiente = 0;
	}
	else
	{
		coeficiente = (y1 - y2)/(x1 - x2);
		independiente = y1 - (coeficiente*x1);
	}
}

int Recta::evaluar(int x)
{
	return (coeficiente*x)+independiente;
}

#endif
