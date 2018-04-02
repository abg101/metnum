#include "Matriz.h"


Matriz::Matriz()
{
    filas = 0;
    columnas = 0;
}

Matriz::Matriz(int filas, int columnas)
{
    this->construct(filas, columnas);
}

Matriz::~Matriz()
{
   
}

int Matriz::Filas()
{
    return filas;
}

int Matriz::Columnas()
{
    return columnas;
}

void Matriz::construct(int filas, int columnas)
{
    if (filas < 1 || columnas < 1) {
        throw "La dimension de la matriz no puede ser cero o negativa";
    }

    this->filas = filas;
    this->columnas = columnas;

    unordered_map<int,double> mapa;
    vector<unordered_map<int,double> > vec(filas,mapa);

    filas_ptr = vec;
    nnz = 0;
}


Matriz & Matriz::Set(double val, int fila, int col)
{
    this->validarCoordenadas(fila, col);

   // pair<int,double> par = make_pair(col, val);
  

        if (abs(val) < epsilon){
           if ((filas_ptr[fila-1].count(col)) > 0){
            
                filas_ptr[fila-1].erase(col);
                --nnz;
                }
        }else{
            filas_ptr[fila-1][col] = val;
            ++nnz;
        }

    return *this;
}



double Matriz::Get(int fila, int col) const
{
    this->validarCoordenadas(fila, col);

    if ((filas_ptr[fila-1].count(col)) > 0)
    {
        return (filas_ptr[fila-1].at(col));
    }

    return 0;
}

vector<double> Matriz::multiply(const vector<double> & x) const
{

}


Matriz Matriz::multiply(const Matriz & b) const
{

}

void imprimirvector80(std::vector<double>& v) {
    for (int i = 0; i < v.size(); ++i)
    {
        cout << v[i] << "," << endl;
    }
}

vector<double> Matriz::valores(){

}



void Matriz::escalar(double k)
{
     

    for (int i = 0; i < filas; i++)
    {
        for(auto it = filas_ptr[i-1].begin(); it != filas_ptr[i-1].end(); ++it){

            it->second = (it->second)*k;
        }
     
    }
}


Matriz Matriz::add(const Matriz & mat) const
{
 
}

Matriz Matriz::sub(const Matriz & mat) const
{

}


void Matriz::validarCoordenadas(int fila, int col) const
{
    if (fila < 1 || col < 1 || fila > this->filas || col > this->columnas) {
        throw "Coordenadas fuera del rango";
    }
}


void Matriz::insert(int index, int fila, int col, double val)
{

}


void Matriz::remove(int index, int fila)
{
 
}

bool operator == (const Matriz & a, const Matriz & b)
{
    return a.filas_ptr == b.filas_ptr;
}


bool operator != (const Matriz & a, const Matriz & b)
{
    return !(a == b);
}


ostream & operator << (ostream & os, const Matriz & matrix)
{
    for (int i = 1; i <= matrix.filas; i++) {
        for (int j = 1; j <= matrix.columnas; j++) {
            if (j != 1) {
                os << " ";
            }

            os << matrix.Get(i, j);
        }

        if (i < matrix.filas) {
            os << endl;
        }
    }

    return os;
}

Matriz operator * (const Matriz & a, const Matriz & b) {
    
    return a.multiply(b);
}

vector<double> operator * (const Matriz & a, const vector<double> & b) {

    return a.multiply(b);
}

Matriz operator + (const Matriz & a, const Matriz & b){
  
  return a.add(b);

}

Matriz operator - (const Matriz & a, const Matriz & b){

    return a.sub(b);
}

void Matriz::columnaPorMenosPSobreSuGrado(vector<int>& grados, double p){

   for (int i = 0; i < filas; ++i)
    {   
        for(auto it = filas_ptr[i].begin(); it != filas_ptr[i].end(); ++it){
            it->second = (-1.0)*(it->second)*p/grados[it->first-1];
        }
        //vals[i] = (-1)*vals[i]*p/grados[cols[i]-1];
    }
}

void Matriz::resolver(vector<double> & ranking, vector<double> &b){


    this->eliminacionGaussiana(b);


    this->backwardSubstitution(ranking,b);

}

int binary_search_find_index(vector<int> v, int data) {

}

int Matriz::IndiceFila(int j, int k){


    
}

void Matriz::eliminacionGaussiana(vector<double>& b){

    for (int k = 0; k < filas; ++k)
    {   
        for (int i = k+1; i < filas; ++i)
          {
              if (filas_ptr[i].count(k+1) > 0)
              {
                  double coef =  (double)(this->Get(i+1,k+1)) / (this->Get(k+1,k+1));//filas_ptr[i][k+1]/filas_ptr[k][k+1];


                  //recorro fila k

                    for (int j = k; j < columnas; ++j)
                    {
                         
                         double resta = this->Get(i+1,j+1) - coef*(this->Get(k+1,j+1));
                 
                        this->Set(resta,i+1,j+1);
                   
                    }

                    b[i] = b[i] - coef*b[k] ;
  
              }
          }  
    }

}

void Matriz::backwardSubstitution(vector<double>& ranking, vector<double>& b){

    double acum = 0.0;
    //int i = this->vals.size()-1; // empezamos desde el ultimo.
    for (int k = filas-1; k >= 0; --k)
    {
        // indice en vals donde empieza la fila k y por lo tanto a_k_k.
        

        //recorro la fila en busca de coeficientes no nulos que multipliquen a las
        // x_l mayores que a x_i_i.
        for (int l = k+1; l < columnas ; ++l)
        {   
            acum = acum +  this->Get(k+1,l+1)*ranking[l];
           
        }
        //resto b_i menos el acum y lo divido por el coef de a_i_i.

        ranking[k] = (double) ((b[k]-acum) / this->Get(k+1,k+1));//  vals[i]);
        acum = 0.0;


        //imprimirvector80(ranking);

    }

 }
