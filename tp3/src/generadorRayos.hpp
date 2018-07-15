#include <cmath>
#include <cstdlib>
#include <tuple>
#include <cassert>
#include "Recta.hpp"

int randomEntre(int l, int u)
{
	int r = std::trunc(std::rand());
	int res = (r % (u - l)) + l;
	assert(res <= u);
	assert(res >= l);
	return res;
}

tuple<int, int> randPos(int lado, int filas, int columnas)
{
	int f, c;
	switch(lado)
	{
		case(0): // izquierdo
		{
			f = randomEntre(0, filas-1);
			c = 0;
			break;
		}
		case(1): // arriba
		{
			f = 0;
			c = randomEntre(0, columnas-1);
			break;
		}
		case(2): // derecha
		{
			f = randomEntre(0,filas-1);
			c = columnas-1;
			break;
		}
		case(3): // abajo
		{
			f = filas-1;
			c = randomEntre(0,columnas-1);
			break;
		}
		default: //lado no esta en un valor valido
			break;
	}
	assert(f < filas);
	assert(c < columnas);
	return std::make_tuple(f,c);
}


tuple< double, Matriz<double> > simularRayo(Matriz<double> imagen, int n, int m, int fe, int ce, int fs, int cs)
{
    // [t, D] = simularRayo(I, n, m, f_ini, c_ini, f_fin, c_fin, [dibujar])
    //
    // Simula un rayo del tomografo a traves de la imagen I, discretizada en
    // n filas y m columnas. El rayo tiene como punto inicial el punto
    // (f_ini, c_ini) y como punto final el punto (f_ini, c_ini).
    // Retorna: el tiempo total t de la emision del rayo y las distancias
    // recorridas D(i,j) en cada punto de la discretizacion.
    // El parametro dibujar es opcional y si vale 1 se dibuja el rayo sobre
    // la imagen.

	int filas = imagen.filas();
	int columnas = imagen.columnas();
	int df = filas/n;
	int dc = columnas/m;
	double t = 0;
	Matriz<double> D(n,m,0);
	
	//Calculo la recta que representa el rayo (y su inversa)

	Recta pi(fe, ce, fs, cs); 	
	Recta pj(ce, fe, cs, fs); 		

	int i1 = pj.evaluar(0);
	int i2 = pj.evaluar(columnas-1);
    int i_min = std::max(0, std::min(filas-1, std::min(i1,i2)));
    int i_max = std::max(0, std::min(filas-1, std::max(i1,i2)));

	// recorro cada fila (entre i_min e i_max) y veo que columnas toca el rayo

	for(int i = i_min; i <= i_max; i++)
	{
		int j1 = pi.evaluar(i-1);
		int j2 = pi.evaluar(i);
		int j_min = std::max(1, std::min(columnas-1, (int) std::ceil(std::min(j1,j2))));
		int j_max = std::max(1, std::min(columnas-1, (int) std::ceil(std::max(j1,j2))));
		for(int j = j_min; j <= j_max; j++)
		{
			t += imagen[i][j] + 1;
			int n_i = std::min(n-1, (int) std::trunc((double)i/df));
			int m_j = std::min(m-1, (int) std::trunc((double)j/dc));
			D[n_i][m_j] = D[n_i][m_j] + 1; 
		}
	}
	
	return std::make_tuple(t, vectorizar(D));
}

tuple< Matriz<double>, Matriz<double> > generarRayos(Matriz<double> imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();
	Matriz<double> rayos(k, n*m,0);
	Matriz<double> tiempos(k, 1);
	int p = std::trunc(m/20);
	for(int i = 0; i < k; i++)
	{
		//sale y entra resultan siempre distintos
		int entra = randomEntre(0,3);
		int sale = (entra + randomEntre(0,2) + 1) % 4;
	
		auto posEntrada = randPos(entra, filas, columnas);
		auto posSalida = randPos(sale, filas, columnas);

		int fe = std::get<0>(posEntrada);
		int ce = std::get<1>(posEntrada);
		int fs = std::get<0>(posSalida);
		int cs = std::get<1>(posSalida);

		auto res = simularRayo(imagen, n, m, fe, ce, fs, cs);
		double t = std::get<0>(res);
		Matriz<double> simRayo = std::get<1>(res);
	    
		rayos.set_fil(i,simRayo);
		tiempos[i][0] = t;

	}
	return make_tuple(tiempos, rayos);
}


