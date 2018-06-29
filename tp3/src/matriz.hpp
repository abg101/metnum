#ifndef MATRIZ_HPP
#define MATRIZ_HPP
#include <memory>
#include <iostream>
#include <cassert>
#include <algorithm>

template<class T>
class Matriz
{
    public:
        Matriz();
        Matriz(int,int);
        Matriz(int,int,T);
        Matriz(const Matriz&);
        Matriz(Matriz&& arg);
        ~Matriz();

        T* operator[](int i){
            assert(i >= 0 && i < cant_filas);
            return (i*cant_columnas)+datos;
        };

        const T* operator[](int i) const{
            assert(i >= 0 && i < cant_filas);
            return (i*cant_columnas)+datos;
        };
            
        inline int columnas() const{return cant_columnas;};
        inline int filas() const{return cant_filas;};

        Matriz traspuesta() const;
        void   trasponer();

        Matriz copy_col(int) const;
        Matriz copy_fil(int) const;
        Matriz copiar_rango(int,int,int,int) const;
        void set_rango(int,int,int,int,const Matriz&);

        void set_fil(int,const Matriz&);
        void set_col(int,const Matriz&);
        
        Matriz& operator+=(const Matriz&);
        Matriz& operator-=(const Matriz&);
        Matriz& operator/=(T);
        Matriz& operator*=(T);

        Matriz& operator=(const Matriz&);
        Matriz& operator=(Matriz&&);

        T mult_fila(int,const Matriz&) const;
        T mult_columna(int,const Matriz&) const;
        T mult_fila(int,const Matriz&,int) const;
        T mult_columna(int,const Matriz&,int) const;


    private:
        T* datos;
        int cant_columnas;
        int cant_filas;
};

//Constructores

//Constructor basico, los valores de la matriz son basura
//Asume que va a ser modificada inmediatamente luego de ser creada
template<class T>
Matriz<T>::Matriz(int fil, int col)
{
    assert(fil > 0 && col > 0);
    cant_columnas = col;
    cant_filas = fil;
    datos = new T[fil*col];
}

//Constructor que permite asignar valores a la matriz al crearla
//El valor filler se usa para rellenarla
template<class T>
Matriz<T>::Matriz(int fil, int col, T filler)
{
    assert(fil > 0 && col > 0);
    cant_columnas = col;
    cant_filas = fil;
    datos = new T[fil*col];
    for(int i = 0;i < fil*col;i++)
    {
        datos[i] = filler;
    }
}

//Constructor por copia
template<class T>
Matriz<T>::Matriz(const Matriz<T>& otra)
{	
    cant_filas = otra.cant_filas;
    cant_columnas = otra.cant_columnas;

    datos = new T[cant_filas*cant_columnas];
    for(int i = 0;i < otra.cant_filas*otra.cant_columnas;i++)
        datos[i] = otra.datos[i];
}

//Constructor move(ver docs de C++ para ver que es esto)
template<class T>
Matriz<T>::Matriz(Matriz&& arg):datos(std::move(arg.datos))
{
    arg.datos = NULL;
    cant_filas = std::move(arg.cant_filas);
    cant_columnas = std::move(arg.cant_columnas);
};

//Destructor
template<class T>
Matriz<T>::~Matriz()
{
    if(datos != NULL)
        delete[] datos;

    datos = NULL;
}


//Operador de streams, para escribir la matriz en un archivo/consola.
//Pone un componente de la matriz por linea

template<class T>
std::ostream& operator<<(std::ostream& os, const Matriz<T>& obj)
{
	for(int i = 0;i < obj.filas();i++)
        {
		os<<'|';
		for(int j = 0;j < obj.columnas();j++)
                {
			os<<obj[i][j];
                        if(j < obj.columnas()-1)
			    os<<" ";
		}
		os<<'|'<<'\n';
	}
	return os;
}

//Operadores aritméticos
//Suma, resta y producto entre matrices
//Producto con escalares
//(por ahora solo de la forma escalar*matriz, no matriz*escalar)

//El producto entre matrices usa el algoritmo de multiplicación bruto
//Hay un algoritmo mejor asintóticamente, pero no se si vale la pena por ahora
//Ni tampoco sé a partir de qué cantidad de elementos empieza a ser mejor

//La suma y la resta modifican a la matriz original
//Ambos productos no modifican la matriz original
//Esto se puede cambiar, despues vemos que conviene más

template<class T>
Matriz<T> operator+(const Matriz<T>& lhs,const Matriz<T>& rhs)
{
    Matriz<T> res(lhs);
    res += rhs;
    return res;
};

template<class T>
Matriz<T> operator-(const Matriz<T>& lhs,const Matriz<T>& rhs)
{
    Matriz<T> res(lhs);
    res -= rhs;
    return res;
};

template<class T>
Matriz<T> mult_canonica(const Matriz<T>& lhs,const Matriz<T>& rhs)
{
    assert(lhs.columnas() == rhs.filas());

    Matriz<T> res(lhs.filas(), rhs.columnas(),0.0);

    for(int i = 0;i < lhs.filas();i++)
    	for(int k = 0; k < rhs.columnas();k++)
		for(int j = 0;j < rhs.filas();j++)
			res[i][k] += lhs[i][j]*rhs[j][k];

    return res;
}

template<class T>
Matriz<T> mult_tiled(const Matriz<T>& lhs,const Matriz<T>& rhs, int t)
{
    assert(t > 1);
    assert(lhs.columnas() == rhs.filas());

    Matriz<T> res(lhs.filas(), rhs.columnas(),0.0);

    for(int i = 0;i < lhs.filas();i+=t)
    	for(int j = 0; j < rhs.columnas();j+=t)
    	    for(int k = 0;k < rhs.filas();k+=t)
                for(int x = i;x < std::min(i+t, lhs.filas());x++)
                    for(int y = j;y < std::min(j+t, rhs.columnas());y++)
                        for(int z = k;z < std::min(k+t, rhs.filas());z++)
                            res[x][y] += lhs[x][z] * rhs[z][y];

    return res;
}



template<class T>
Matriz<T> operator*(const Matriz<T>& lhs,const Matriz<T>& rhs)
{
    assert(lhs.columnas() == rhs.filas());

    int t = 8;
    if(rhs.columnas() < t || lhs.filas() < t || rhs.filas() < t)
        return mult_canonica(lhs,rhs);
    else
        return mult_tiled(lhs, rhs, t);
}


template<class T>
Matriz<T> operator*(T lhs, const Matriz<T>& rhs)
{
	Matriz<T> res(rhs);
	for(int i = 0;i < rhs.filas();i++)
		for(int k = 0; k < rhs.columnas();k++)
         	   res[i][k] *= lhs;

	return res;
}

template<class T>
Matriz<T> operator*(const Matriz<T>& lhs, T rhs)
{
	Matriz<T> res(lhs);
	for(int i = 0;i < lhs.filas();i++)
		for(int k = 0; k < lhs.columnas();k++)
         	   res[i][k] *= rhs;

	return res;
}

template<class T>
Matriz<T> operator/(T lhs, const Matriz<T>& rhs)
{
	Matriz<T> res(rhs);
	for(int i = 0;i < rhs.filas();i++)
		for(int k = 0; k < rhs.columnas();k++)
         	   res[i][k] /= lhs;

	return res;
}

template<class T>
Matriz<T> operator/(const Matriz<T>& lhs, T rhs)
{
	Matriz<T> res(lhs);
	for(int i = 0;i < lhs.filas();i++)
		for(int k = 0; k < lhs.columnas();k++)
         	   res[i][k] /= rhs;

	return res;
}

template<class T>
Matriz<T>& Matriz<T>::operator+=(const Matriz<T>& rhs)
{
    assert(cant_filas == rhs.filas() && cant_columnas == rhs.columnas());

    for(int i = 0; i < cant_filas;i++)
        for(int j = 0; j < cant_columnas;j++)
            (*this)[i][j] += rhs[i][j];

    return (*this);
}

template<class T>
Matriz<T>& Matriz<T>::operator-=(const Matriz<T>& rhs)
{
    assert(cant_filas == rhs.filas() && cant_columnas == rhs.columnas());

    for(int i = 0; i < cant_filas;i++)
        for(int j = 0; j < cant_columnas;j++)
            (*this)[i][j] -= rhs[i][j];

    return (*this);
}

template<class T>
Matriz<T>& Matriz<T>::operator/=(T div)
{
    for(int i = 0; i < cant_filas;i++)
        for(int j = 0; j < cant_columnas;j++)
            (*this)[i][j] /= div;

    return (*this);
}

template<class T>
Matriz<T>& Matriz<T>::operator*=(T div)
{
    for(int i = 0; i < cant_filas;i++)
        for(int j = 0; j < cant_columnas;j++)
            (*this)[i][j] *= div;

    return (*this);
}



template<class T>
Matriz<T>& Matriz<T>::operator=(const Matriz<T>& rhs)
{
    if(this == &rhs)
        return *this;

    if(datos == NULL || 
        (cant_filas != rhs.cant_filas && cant_columnas != rhs.cant_columnas))
    {
        if(datos != NULL)
            delete[] datos;
        datos = new T[rhs.cant_filas*rhs.cant_columnas];
    }

    for(int i = 0;i < rhs.cant_filas*rhs.cant_columnas;i++)
    {
        datos[i] = rhs.datos[i];
    }
 
    cant_filas = rhs.cant_filas;
    cant_columnas = rhs.cant_columnas;
    return (*this);
}

template<class T>
Matriz<T>& Matriz<T>::operator=(Matriz&& otro)
{
    datos = std::move(otro.datos);
    otro.datos = NULL;
    cant_filas = std::move(otro.cant_filas);
    cant_columnas = std::move(otro.cant_columnas);
    return (*this);
}
//Devuelve una copia de la matriz original, traspuesta
template<class T>
Matriz<T> Matriz<T>::traspuesta() const
{
    Matriz<T> res(cant_columnas, cant_filas);
    for(int i = 0;i < cant_filas;i++)
      for(int j = 0;j < cant_columnas;j++)
          res[j][i] = (*this)[i][j];

    return res;
}


//Devuelve una submatriz de la original, delimitada por los parametros
template<class T>
Matriz<T> Matriz<T>::copiar_rango(int fi,int ff,int ci,int cf) const
{
    assert(fi >= 0 && fi <= ff &&
           ff >= 0 && ff < cant_filas &&
           ci >= 0 && ci <= cf &&
           cf >= 0 && cf < cant_columnas);

    Matriz<T> res(ff-fi+1,cf-ci+1);
    for(int i = fi;i <= ff;i++)
       for(int j = ci;j <= cf;j++)
          res[i-fi][j-ci] = (*this)[i][j];

    return res;
}

template<class T>
void Matriz<T>::set_rango(int fi,int ff,int ci,int cf,const Matriz<T>& otra)
{
    assert(fi >= 0 && fi <= ff &&
           ff >= 0 && ff < cant_filas &&
           ci >= 0 && ci <= cf &&
           cf >= 0 && cf < cant_columnas);

    assert(otra.filas() == (ff-fi+1) &&
           otra.columnas() == (cf-ci+1));

    for(int i = fi;i <= ff;i++)
       for(int j = ci;j <= cf;j++)
          (*this)[i][j] = otra[i-fi][j-ci];
}
//Devuelve una copia de la columna i
template<class T>
Matriz<T> Matriz<T>::copy_col(int i) const
{
    assert(0 <= i && i < this->columnas());
    return this->copiar_rango(0,this->filas()-1,i,i);
}

//Devuelve una copia de la fila i
template<class T>
Matriz<T> Matriz<T>::copy_fil(int i) const
{
    assert(0 <= i && i < this->filas());
    return this->copiar_rango(i,i,0,this->columnas()-1);
}

//Traspone la matriz
template<class T>
void Matriz<T>::trasponer()
{
    Matriz<T> temp = (*this);
    for(int j = 0;j < cant_filas;j++)
      for(int i = 0;i < cant_columnas;i++)
          (*this)[j][i] = temp[i][j];
}

//Copia el vector a en la fila i. a es vector columna
template<class T>
void Matriz<T>::set_fil(int i,const Matriz<T>& a)
{
    assert(i >= 0 && i < cant_filas);
    assert(a.filas() == cant_columnas);

    for(int j = 0;j < cant_columnas;j++)
        (*this)[i][j] = a[j][0];
}

//Copia el vector a en la columna i. a es vector columna
template<class T>
void Matriz<T>::set_col(int i,const Matriz<T>& a)
{
    assert(i >= 0 && i < cant_columnas);
    assert(a.filas() == cant_filas);

    for(int j = 0;j < cant_filas;j++)
        (*this)[j][i] = a[j][0];
}

template<class T>
T Matriz<T>::mult_fila(int i, const Matriz<T>& otra) const
{
    assert(i >= 0 && i < cant_filas);
    assert(cant_columnas == otra.filas());

    T res = 0.0;
    for(int j = 0;j < cant_columnas;j++)
        res += (*this)[i][j] * otra[j][0];

    return res;
}

template<class T>
T Matriz<T>::mult_columna(int i, const Matriz<T>& otra) const
{
    assert(i >= 0 && i < cant_columnas);
    assert(cant_filas == otra.columnas());

    T res = 0.0;
    for(int j = 0;j < cant_filas;j++)
        res += (*this)[j][i] * otra[0][j];

    return res;
}

template<class T>
T Matriz<T>::mult_fila(int i, const Matriz<T>& otra, int k) const
{
    assert(i >= 0 && i < cant_filas &&
            k >= 0 && k < otra.columnas());
    assert(cant_columnas == otra.filas());

    T res = 0.0;
    for(int j = 0;j < cant_columnas;j++)
        res += (*this)[i][j] * otra[j][k];

    return res;
}

template<class T>
T Matriz<T>::mult_columna(int i, const Matriz<T>& otra, int k) const
{
    assert(i >= 0 && i < cant_columnas &&
            k >= 0 && k < otra.filas());
    assert(cant_filas == otra.columnas());

    T res = 0.0;
    for(int j = 0;j < cant_filas;j++)
        res += (*this)[j][i] * otra[k][j];

    return res;
}

template<class T>
Matriz<T> mult_bloque(const Matriz<T>& a, const Matriz<T>& b,int afi,int aff,int aci,int acf,int bfi,int bff,int bci,int bcf)
{
    assert(acf-aci == bff-bfi);

    Matriz<T> res(aff-afi+1,bcf-bci+1,0);
    for(int i = afi;i <= aff;i++)
        for(int j = bci;j <= bcf;j++)
            for(int k = aci;k <= acf;k++)
                res[i-afi][j-bci] += a[i][k]*b[k][j];

    return res;
}

typedef Matriz<double> Matriz_d;
typedef Matriz<float> Matriz_f;
#endif
