#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <limits>
#include "metodos_reduc.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>// max y transform
#include "IOUtils.hpp"

using namespace std;
/*
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
*/ 

/*void show_matrix(double *A, int n,int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%2.5f ", A[i * n + j]);
        printf("\n");
    }
}
*/

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
    
    A.set_fil(0,temp1);
    A.set_fil(2,temp2);
    A.set_fil(3,temp3);

    cout << "A:       " << endl;
    cout << A[0][0] << A[0][1]<< A[0][2]<< A[0][3]<< endl;
    cout << A[1][0] << A[1][1]<< A[1][2]<< A[1][3]<< endl;
    cout << A[2][0] << A[2][1]<< A[2][2]<< A[2][3]<< endl;

    Matriz<double> res = resolver_sistema(A);

    cout << "res:       " << endl;
    cout << res[0][0] << res[0][1]<< res[0][2]<< res[0][3]<< endl;
    cout << res[1][0] << res[1][1]<< res[1][2]<< res[1][3]<< endl;
    cout << res[2][0] << res[2][1]<< res[2][2]<< res[2][3]<< endl;    

    return 0;

}
