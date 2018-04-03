#ifndef Matriz_H
#define Matriz_H
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
//#include <math.h>
#include <cmath>
#include <assert.h>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <map>

using namespace std;


#define epsilon 0.00000001


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

   // void Resize(long,long);

    vector<double> multiply(const vector<double> & x) const;

    Matriz multiply(const Matriz & m) const;

    void escalar(double k);

    Matriz add(const Matriz & m) const;

    Matriz sub(const Matriz & m) const;

    void resolver(vector<double> & ranking, vector<double> &b);

    void eliminacionGaussiana(vector<double>& b);

    void backwardSubstitution(vector<double>& ranking, vector<double>& b);

    int IndiceFila(int j, int k);

    friend bool operator == (const Matriz & a, const Matriz & b);

    friend bool operator != (const Matriz & a, const Matriz & b);

    friend Matriz operator + (const Matriz & a, const Matriz & b);

    friend Matriz operator - (const Matriz & a, const Matriz & b);

    friend Matriz operator * (const Matriz & a, const Matriz & b);

    friend vector<double> operator * (const Matriz & a, const vector<double> & b);

    friend ostream & operator << (ostream & os, const Matriz & matrix);

    friend void prlong(const Matriz & matrix, ostream & os);

    vector<double> valores();

    void columnaPorMenosPSobreSuGrado(vector<int>& grados, double p);


private:
    
    vector<map<int,double> >  filas_ptr;
    int nnz;

    int filas;
    int columnas;

    void construct(int fila, int columna);
    void validarCoordenadas(int fila, int col) const;
    void insert(int index, int fila, int col, double val);
    void remove(int index, int fila);


};

#endif
