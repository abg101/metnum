#include "PageRank.h"
#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////
PageRank::PageRank()
{
    matrizDeConectividad = Matriz();
    gradoDePaginas = vector<int>();
    ranking = vector<double>();
    cantDePaginas = 0;
}

PageRank::PageRank(Matriz& W, vector<int> & grado_de_paginas){

    matrizDeConectividad = W;
    gradoDePaginas = grado_de_paginas;
    cantDePaginas = W.Filas();
    ranking = vector<double>(cantDePaginas);

}

PageRank::~PageRank()
{
}

bool contienePagina(vector<pair<int, int> > paginas, int pagOrigen)
{
    bool res = false;
    for(int i = 0; i < paginas.size(); i++) if(paginas[i].first == pagOrigen) res=true;
    return res;
}

int obtenerIndicePagina(vector<pair<int, int> > paginas, int pagOrigen)
{
    int res;
    for(int i = 0; i < paginas.size(); i++) if(paginas[i].first == pagOrigen) res=i;
    return res;
}

int PageRank::obtenerGradoPagina(int pagOrigen)
{
    int res;
    res = gradoDePaginas[pagOrigen-1];
    return res;
}


Matriz& PageRank::GetMatrizDeConectividad()
{
    return matrizDeConectividad;
}

vector<double >& PageRank::GetRanking()
{
    return ranking;
}

vector<int >& PageRank::GetGradoDePaginas()
{
    return gradoDePaginas;
}

int PageRank::getCantDePaginas()
{
    return cantDePaginas;
}

double norma(vector<double> &v)
{
    double res = 0;
    for(long i = 0; i < v.size(); i++) res += v[i]*v[i];
    return (double) sqrt(res);
}

void divEscalar(vector<double> &v, double alpha)    
{
    if(alpha!=0)
        for(int i = 0; i < v.size(); i++) v[i] = (double) (v[i]/alpha);
}

double normaManhattan(vector<double> &v)
{
   double res = 0;
    for(int i = 0; i < v.size(); i++)res += abs((double)v[i]);
    return res;
}

void normalizar(vector<double> &V){

    double nrm = (double) normaManhattan(V);
    divEscalar(V, nrm);
}

void PageRank::calcularRanking(double p)
{

    vector<double> b(this->cantDePaginas,1);

    // matrizDeCOnectividad = -pAD
    this->matrizDeConectividad.columnaPorMenosPSobreSuGrado(this->gradoDePaginas,p);

    //matrizDeConectividad = matrizDeConectividad + i. La que queremos resolver.
    for (int i = 1; i <= this->cantDePaginas; ++i)
        {
            this->matrizDeConectividad.Set(1,i,i);
        }    

    vector<double> solucion(cantDePaginas);

    this->matrizDeConectividad.resolver(this->ranking,b);

    normalizar(ranking);
    cout << "asd" << endl; 

}
