#ifndef METODOS_REDUC_HPP
#define METODOS_REDUC_HPP
#include "matriz_aux.hpp"
#include "normas_aux.hpp"


//Metodo de la potencia
//Asume que vec y autovec son filas
//Vec es el vector inicial
//Se ejecuta hasta que no cambian mucho los valores generados o hasta niter
double met_potencia(const Matriz<double>& b, const Matriz<double>& vec, int niter, Matriz<double>& autovec)
{
    Matriz<double> anterior(autovec.filas(),autovec.columnas(),0);
    autovec = vec;
    for(int i = 0;i < niter;i++)
    {
        Matriz<double> temp = b*autovec;
        autovec = temp/norma_2_vec(temp);
        if(son_parecidas(autovec,anterior,1.0))
            break;
	anterior = autovec;
    }
    double coef = ((norma_2_vec(autovec)*norma_2_vec(autovec)));

    return (autovec.traspuesta()*b*autovec)[0][0]/coef;
}

//Devuelve una matriz con los autovectores de datos, en las columnas
Matriz<double> obtener_autovectores(const Matriz<double>& datos, int niter)
{
    Matriz<double> b(datos);

    Matriz<double> x(gen_matriz_random(datos.filas(),1)); 
    x = x/norma_2_vec(x);

    Matriz<double> autoval(datos.filas(),1,0.0);
    Matriz<double> res(datos.filas(),datos.filas(),0.0);

    Matriz<double> temp(x);
    int i = 0;
    while(i < datos.filas())
    {
        autoval[i][0] = met_potencia(b, x, niter, temp);
        if(autoval[i][0] == autoval[i][0])
        {
            b -= autoval[i][0]*(temp*temp.traspuesta());
            res.set_col(i, temp/norma_2_vec(temp));
            i++;
        }
        else
        {
            x = gen_matriz_random(datos.filas(),1);
            x = x/norma_2_vec(x);
        }
    }
    return res; 
}

//Devuelve una matriz con los primeros j autovectores de datos, en las columnas
Matriz<double> obtener_autovectores(const Matriz<double>& datos, int niter,int j)
{
    Matriz<double> b(datos);

    Matriz<double> x(gen_matriz_random(datos.filas(),1)); 
    x = x/norma_2_vec(x);

    Matriz<double> autoval(datos.filas(),1,0.0);
    Matriz<double> res(datos.filas(),j,0.0);

    Matriz<double> temp(x);
    int i = 0;
    while(i < j)
    {
        autoval[i][0] = met_potencia(b,x,niter,temp);
        if(autoval[i][0] == autoval[i][0])
        {
            b -= autoval[i][0]*(temp*temp.traspuesta());
            res.set_col(i,temp);
            i++;
        }
        else
        {
            x = gen_matriz_random(datos.filas(),1);
            x = x/norma_2_vec(x);
        }
    }
    return res; 
}

//Devuelve un vector columna con los autovalores de datos
Matriz<double> obtener_autovalores(const Matriz<double>& datos, int niter)
{
    Matriz<double> b(datos);

    Matriz<double> x(gen_matriz_random(datos.filas(),1)); 
    x = x/norma_2_vec(x);

    Matriz<double> res(datos.filas(),1,0.0);

    int i = 0;
    while(i < datos.filas())
    {
        Matriz<double> temp(x);
        res[i][0] = met_potencia(b,x,niter,temp);
        if(res[i][0] == res[i][0])
        {
            b -= (res[i][0]*temp*temp.traspuesta());
            i++;
        }
        else
        {
            x = gen_matriz_random(datos.filas(),1);
            x = x/norma_2_vec(x);
        }
    }
    return res; 
}

//Devuelve una vector columna con los primeros j autovalores de datos
Matriz<double> obtener_autovalores(const Matriz<double>& datos, int niter,int j)
{
    Matriz<double> b(datos);

    Matriz<double> x(gen_matriz_random(datos.filas(),1)); 
    x = x/norma_2_vec(x);

    Matriz<double> res(j,1,0.0);

    int i = 0;
    while(i < j)
    {
        Matriz<double> temp(x);
        res[i][0] = met_potencia(b,x,niter,temp);
        if(res[i][0] == res[i][0])
        {
            b -= res[i][0]*(temp*temp.traspuesta());
            i++;
        }
        else
        {
            x = gen_matriz_random(datos.filas(),1);
            x = x/norma_2_vec(x);
        }
    }
    return res; 
}


//Devuelve la media de la matriz, sumando los elementos por columnas
Matriz<double> media(const Matriz<double>& a)
{
    Matriz<double> res(1,a.columnas(),0.0);
    for(int i = 0;i < a.filas();i++)
        for(int j = 0;j < a.columnas();j++)
            res[0][j] += a[i][j];

    res /= double(a.filas());
    return res;
}

#endif
