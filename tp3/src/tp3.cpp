#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <limits>
#include "matriz_aux.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>// max y transform
#include "IOUtils.hpp"
#include "cuadradosMinimos.hpp"

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
    Matriz<double> A(3,3,0);
    Matriz<double> b(3,1,0);
    Matriz<double> temp1(3, 1, 0);
    Matriz<double> temp2(3, 1, 0);
    Matriz<double> temp3(3, 1, 0); 
   
    temp1[0][0] = -1;
    temp1[1][0] = 3;
    temp1[2][0] = 2;
    temp2[0][0] = 2;
    temp2[1][0] = -5;
    temp2[2][0] = 1;
    temp3[0][0] = 3;
    temp3[1][0] = 1;
    temp3[2][0] = -6;

    b[0][0] = 12;
    b[1][0] = -10;
    b[2][0] = 4;
    
    A.set_fil(0,temp1);
    A.set_fil(1,temp2);
    A.set_fil(2,temp3);

    cout << "A:       " << endl;
    cout << "|" << A[0][0] << " " << A[0][1]<< " " << A[0][2] << " |"<< b[0][0]<< "|" << endl;
    cout << "|" << A[1][0] << " " << A[1][1] << " "<< A[1][2] << " |" << b[1][0]<< "|" << endl;
    cout << "|" << A[2][0] << " " << A[2][1] << " "<< A[2][2] << " |" << b[2][0]<< "|" << endl;

    Matriz<double> res = resolver_gauss(A,b);

    cout << "res:       " << endl;
    cout << "|" << res[0][0] << "|" << endl;
    cout << "|" << res[1][0] << "|" << endl;
    cout << "|" << res[2][0] << "|" << endl;

    Matriz<double> C(5,2,0);
    Matriz<double> z(5,1,0);
    Matriz<double> temp4(2, 1, 0);
    Matriz<double> temp5(2, 1, 0);
    Matriz<double> temp6(2, 1, 0);
    Matriz<double> temp7(2, 1, 0);
    Matriz<double> temp8(2, 1, 0); 
   
    temp4[0][0] = 1;
    temp4[1][0] = -2;
    temp5[0][0] = 1;
    temp5[1][0] = -1;
    temp6[0][0] = 1;
    temp6[1][0] = 0;
    temp7[0][0] = 1;
    temp7[1][0] = 1;
    temp8[0][0] = 1;
    temp8[1][0] = 2;

    z[0][0] = 0.5;
    z[1][0] = 0.5;
    z[2][0] = 2;
    z[3][0] = 3.5;
    z[4][0] = 3.5;
    
    C.set_fil(0,temp4);
    C.set_fil(1,temp5);
    C.set_fil(2,temp6);
    C.set_fil(3,temp7);
    C.set_fil(4,temp8);

    cout << "C:       " << endl;
    cout << "|" << C[0][0] << " " << C[0][1] <<  " |"<< z[0][0]<< "|" << endl;
    cout << "|" << C[1][0] << " " << C[1][1] << " |" << z[1][0]<< "|" << endl;
    cout << "|" << C[2][0] << " " << C[2][1] << " |" << z[2][0]<< "|" << endl;
    cout << "|" << C[3][0] << " " << C[3][1] <<  " |"<< z[3][0]<< "|" << endl;
    cout << "|" << C[4][0] << " " << C[4][1] << " |" << z[4][0]<< "|" << endl;


    Matriz<double> res2 = resolverEN(C,z);

    cout << "res:       " << endl;
    cout << "|" << res2[0][0] << "|" << endl;
    cout << "|" << res2[1][0] << "|" << endl;

    return 0;

}
