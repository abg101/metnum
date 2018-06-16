#ifndef Matriz_H
#define Matriz_H
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <assert.h>
#include <utility>
#include <list>

using namespace std;


//#define epsilon 0.00000000001
const double epsilon = 0.000000001;

/// Indexa de 1 a n/m ///

class Matriz
{

public:

    Matriz();

    Matriz(int, int);

    ~Matriz();

    int Filas();

    int Columnas();

    Matriz & Set(double val, int fila, int col); 

    double Get(int, int) const;

    void escalar(double k);    

    vector<double> multiply(const vector<double> & x) const;

    void resolver(vector<double> & ranking, vector<double> &b);

    void eliminacionGaussiana(vector<double>& b);

    void backwardSubstitution(vector<double>& ranking, vector<double>& b);

    friend bool operator == (const Matriz & a, const Matriz & b);

    friend bool operator != (const Matriz & a, const Matriz & b);

    friend vector<double> operator * (const Matriz & a, const vector<double> & b);

    friend ostream & operator << (ostream & os, const Matriz & matrix);

    void columnaPorMenosPSobreSuGrado(vector<int>& grados, double p);

    int GetNoNulos();


private:
    
    vector<list<pair<int,double> > >  filas_ptr;
    int nnz;

    int filas;
    int columnas;

    void construct(int fila, int columna);
    void validarCoordenadas(int fila, int col) const;


};

#endif
