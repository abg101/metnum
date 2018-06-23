#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <limits>
#include <cmath>
#include <ctime>
#include <algorithm>// max y transform
#include <matriz.hpp>
#include <matriz_aux.hpp>

using namespace std;

double *cholesky(double *A, int n) {
    double *L = new double[n*n];
    if (L == NULL)
        exit(EXIT_FAILURE);
 
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (i+1); j++) {
            double s = 0;
            for (int k = 0; k < j; k++)
                s += L[i * n + k] * L[j * n + k];
            L[i * n + j] = (i == j) ?
                           sqrt(A[i * n + i] - s) :
                           (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
 
    return L;
}
 
void show_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%2.5f ", A[i * n + j]);
        printf("\n");
    }
}

//http://www1.eafit.edu.co/cursonumerico/capitulo3/sesion_1/documentos/eliminacion_gausiana.pdf
int main(int argc, char* argv[]){
    Matriz<double> A(3,4,0);
    Matriz<double> temp1(4, 1, 0);
    Matriz<double> temp2(4, 1, 0);
    Matriz<double> temp3(4, 1, 0); 
   
    temp1[0][0] = 1;
    temp1[1][0] = 2;
    temp1[2][0] = 3;
    temp1[3][0] = 1;
    temp2[0][0] = 4;
    temp2[1][0] = 5;
    temp2[2][0] = 6;
    temp2[3][0] = -2;
    temp3[0][0] = 7;
    temp3[1][0] = 8;
    temp3[2][0] = 10;
    temp3[3][0] = 5;
    
//FALTA
    

    return 0;
}
