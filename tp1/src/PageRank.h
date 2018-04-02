#ifndef PAGERANK_H_INCLUDED
#define PAGERANK_H_INCLUDED
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include "Matriz.h"

using namespace std;


class PageRank
{

public:

    PageRank();

    PageRank(Matriz& W, vector<int> & grado_de_paginas);

    ~PageRank();

    Matriz& GetMatrizDeConectividad();

    vector<double>& GetRanking();

    vector<int >& GetGradoDePaginas();

    int getCantDePaginas();

    int obtenerGradoPagina(int);

    void calcularRanking(double p);	

private:
    Matriz matrizDeConectividad;
    vector<double > ranking;   
    vector<int > gradoDePaginas;    
    int cantDePaginas; 

};



#endif // PAGERANK_H_INCLUDED
