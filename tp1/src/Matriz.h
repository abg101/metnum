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

using namespace std;


/// Indexa de 1 a n/m ///

class Matriz
{

public:

    Matriz();

    Matriz(long, long);

    ~Matriz();

    long Filas();

    long Columnas();

    Matriz & Set(double val, long fila, long col); 

    double Get(long, long) const;    

   // void Resize(long,long);

    vector<double> multiply(const vector<double> & x) const;

    Matriz multiply(const Matriz & m) const;

    void escalar(double k);

    Matriz add(const Matriz & m) const;

    Matriz sub(const Matriz & m) const;

    friend bool operator == (const Matriz & a, const Matriz & b);

    friend bool operator != (const Matriz & a, const Matriz & b);

    friend Matriz operator + (const Matriz & a, const Matriz & b);

    friend Matriz operator - (const Matriz & a, const Matriz & b);

    friend Matriz operator * (const Matriz & a, const Matriz & b);

    friend vector<double> operator * (const Matriz & a, const vector<double> & b);

    friend ostream & operator << (ostream & os, const Matriz & matrix);

    friend void prlong(const Matriz & matrix, ostream & os);
		
	Matriz transpose();

    vector<double> transmultiply(const vector<double> & x) const;

    vector<double> valores();


private:
    
    vector<double>  vals;
    vector<long>  cols;
    vector<long>  filas_ptr;

    long filas;
    long columnas;

    void construct(long fila, long columna);
    void validarCoordenadas(long fila, long col) const;
    void insert(long index, long fila, long col, double val);
    void remove(long index, long fila);


};

#endif
