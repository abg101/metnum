#include "matriz_aux.hpp"
#include "metodos_reduc.hpp"

using namespace std;

const int CANT_POTENCIA = 10000;

//Algoritmo de la diapo para PCA
//Devuelve un vector de autovectores que se utilizan luego como transformacion caracteristica
//Los autovectores son los de la matriz de covarianza de x
//La matriz de autovectores es de TAM_IMAGENES*alfa
//Las columnas son los autovectores
Matriz<double> pca_autovectores(const Matriz<double>& datos, const Matriz<double>& med, unsigned int alfa)
{
    Matriz<double> x(datos);
    Matriz<double> v(datos.filas(), 1, 1);

    // Calculamos X*X^t, siendo X^t*X la matriz de covarianza
    // Estas matrices tiene mismos autovalores pero distintos autovectores
    // Ver informe para la fundamentacion

    x -= v*med;
    x /= std::sqrt(double(datos.filas() - 1));

    Matriz<double> xt(x.traspuesta());

    //Vemos los autovectores de X*X^t y los usamos para averiguar los de X^t*X
    //Sea v_i el iesimo autovector de X*X^t, 
    //hacemos X^t * v_i para obtener el autovector de X^t*X
    Matriz<double> autovec = xt*obtener_autovectores(x*xt, CANT_POTENCIA, alfa);

    Matriz<double> res(datos.columnas(), alfa);
    for(int i = 0;i < autovec.columnas();i++)
        res.set_col(i, autovec.copy_col(i));

    return res;
}


//Idem al anterior pero hallamos los autovectores de xt*x
Matriz<double> pca_autovectores_sin_truco(const Matriz<double>& datos, const Matriz<double>& med, unsigned int alfa)
{
    Matriz<double> x(datos);
    Matriz<double> v(datos.filas(), 1, 1);

    // Calculamos X*X^t, siendo X^t*X la matriz de covarianza
    // Estas matrices tiene mismos autovalores pero distintos autovectores
    // Ver informe para la fundamentacion

    x -= v*med;
    x /= std::sqrt(double(datos.filas() - 1));

    Matriz<double> xt(x.traspuesta());

    //Vemos los autovectores de X*X^t y los usamos para averiguar los de X^t*X
    //Sea v_i el iesimo autovector de X*X^t, 
    //hacemos X^t * v_i para obtener el autovector de X^t*X
    Matriz<double> autovec = obtener_autovectores(xt*x, CANT_POTENCIA, alfa);

    Matriz<double> res(datos.columnas(), alfa);
    for(int i = 0;i < autovec.columnas();i++)
        res.set_col(i, autovec.copy_col(i));

    return res;
}



//Devuelve los autovalores de x*xt
Matriz<double> pca_autovalores(const Matriz<double>& datos, unsigned int alfa)
{
    Matriz<double> x(datos);
    Matriz<double> med = media(x);
    Matriz<double> v(datos.filas(), 1, 1);

    // Calculamos X*X^t, siendo X^t*X la matriz de covarianza
    // Estas matrices tiene mismos autovalores pero distintos autovectores
    // Ver informe para la fundamentacion

    x -= v*med;
    x /= std::sqrt(double(datos.filas() - 1));

    Matriz<double> res = obtener_autovalores(x*x.traspuesta(), CANT_POTENCIA, alfa);
    return res;
}

//Idem al anterior pero calcula los de xt*x
Matriz<double> pca_autovalores_sin_truco(const Matriz<double>& datos, unsigned int alfa)
{
    Matriz<double> x(datos);
    Matriz<double> med = media(x);
    Matriz<double> v(datos.filas(), 1, 1);

    // Calculamos X*X^t, siendo X^t*X la matriz de covarianza
    // Estas matrices tiene mismos autovalores pero distintos autovectores
    // Ver informe para la fundamentacion

    x -= v*med;
    x /= std::sqrt(double(datos.filas() - 1));

    Matriz<double> res = obtener_autovalores(x.traspuesta()*x, CANT_POTENCIA, alfa);
    return res;
}

// Aplica la transformacion caracteristica dada por los autovectores en autovec sobre la matriz datos
// Previamente la normaliza usando la media
// Modifica la matriz datos
void tc(const Matriz<double>& autovec, Matriz<double>& datos, const Matriz<double>& med, int n)
{
    //Preparamos los vectores de los datos
    Matriz<double> canon(datos.filas(),1,1);
    if(datos.filas() > 1)
        datos -= canon*med;
    else
        datos -= med;

    datos /= std::sqrt(double(n-1));

    //Convierto las imagenes de datos usando autovectores_tc
    Matriz<double> tc_datos(datos);
    datos = tc_datos * autovec;
}

// Version mas rapida de tc, solo funciona si datos es un vector fila, no una matriz
Matriz<double> tc_vector(const Matriz<double>& autovec, const Matriz<double>& datos, const Matriz<double>& med, int n)
{
    //Convierto las imagenes de datos usando autovectores_tc
    return ((datos - med)/std::sqrt(double(n-1))) * autovec;
}
// Version mas rapida de tc, solo funciona si datos es una matriz, no un vector
Matriz<double> tc_matriz(const Matriz<double>& autovec, const Matriz<double>& datos, const Matriz<double>& med, int n)
{
    //Preparamos los vectores de los datos
    Matriz<double> canon(datos.filas(),1,1);

    //Convierto las imagenes de datos usando autovectores_tc
    return ((datos - canon*med)/std::sqrt(double(n-1))) * autovec;
}