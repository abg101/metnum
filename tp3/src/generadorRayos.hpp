#include <cmath>
#include <cstdlib>
#include <tuple>

int randomEntre(int l, int u)
{
	int r = std::trunc(std::rand());
	return (r % (l + u + 1)) + l;
}

function [f, c] = randPos(lado, filas, columnas)
{
	int f, c;
	switch(lado)
	{
		case(0): // izquierdo
		{
			f = rand()*filas;
			c = 0;
			break;
		}
		case(1): // arriba
		{
			f = 0;
			c = rand()*columnas;
			break;
		}
		case(2): // derecha
		{
			f = rand()*filas;
			c = columnas;
			break;
		}
		case(3): // abajo
		{
			f = filas;
			c = rand()*columnas;
			break;
		}
		default: //lado no esta en un valor valido
			break;
	}
	return std::make_tuple(f,c);
}


Matriz<int> simularRayo(Matriz<int> imagen, int n, int m, int fe, int ce, int fs, int cs)
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
	int t = 0;
	Matriz<int> distancias(n, m);
	//TODO: llenar de ceros
	
	// Lo siguiente es una chanchada para poder simplificarme la vida usando
    // polyfit m�s adelante
    //if f_ini == f_fin
    //    f_ini = f_ini - 0.5;
    //    f_fin = f_fin + 0.5;
    //end
    //if c_ini == c_fin
    //    c_ini = c_ini - 0.5;
    //    c_fin = c_fin + 0.5;
    //end

	//TODO implementar polnomio? Solo quiero la recta
	//Calculo la recta que representa el rayo (y su inversa)

	//Polinomio pi = polyfit([f_ini, f_fin], [c_ini, c_fin], 1); 
	//Polinomio pj = polyfit([c_ini, c_fin], [f_ini, f_fin], 1); 
	Recta pi(fe, ce, fs, cs);
	Recta pj(ce, fe, cs, fs);

	int i1 = pj.evaluar(1);//en cpp seria 0 y col-1, no?
	int i2 = pj.evaluar(columnas);
    int i_min = std::max(1, std::min(filas-1, std::ceil(std::min(i1,i2))));
    int i_max = std::max(1, std::min(filas-1, std::ceil(std::max(i1,i2))));

	// recorro cada fila (entre i_min e i_max) y veo que columnas toca el rayo

	for(int i = i_min; i < i_max; i++)
	{
		j1 = pi.evaluar(i-1);
		j2 = pi.evaluar(i);
		int j_min = std::max(1, std::min(columnas-1, std::ceil(std::min(j1,j2))));
		int j_max = std::max(1, std::min(columnas-1, std::ceil(std::max(j1,j2))));
		for(int j = j_min; j < j_max; j++)
		{
			int n_i = std::min(n, std::trunc((double)i/df) + 1);
			int m_j = std::min(m, std::trunc((double)i/dc) + 1);
			distancias[n_i][m_j] = D[n_i][m_j] + 1; 
		}
	}
	return distancias;
}

Matriz<int> generarRayos(Matriz<int> imagen, int k, int n, int m)
{
	srand(time(NULL));//TODO: trasladar esto a un lugar donde sea llamado una sola vez
	int filas = imagen.filas();
	int columnas = imagen.columnas();
	Matriz<int> rayos(k, n*m+1);
	p = std::trunc(k/20);
	for(int i = 0; i < k; i++)
	{
		int entra = randomEntre(0,3);
		int sale = (entra + randomEntre(0,3) + 1) % 4;
		auto posEntrada = randPos(entra, filas, columnas);
		auto posSalida = randPos(sale, filas, columnas);

		int fe = std::get<0>(posEntrada);
		int ce = std::get<1>(posEntrada);
		int fs = std::get<0>(posSalida);
		int cs = std::get<1>(posSalida);

		simularRayo(imagen, n, m, fe, ce, fs, cs);

	    //agrego el rayo simulado a mis datos TODO

	}
	return rayos;
}


