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
#include "clasificador.hpp"
#include <algorithm>// max y transform
#include "IOUtils.hpp"

using namespace std;


//Proyecta img en el espacio de autocaras, como dice el paper dado por la catedra
//img es vector fila
Matriz<double> proyectar_cara(const Matriz<double>& autovec, const Matriz<double>& img)
{
    Matriz<double> res(autovec.filas(), 1, 0);
    for(int i = 0;i < autovec.columnas();i++)
    {
        Matriz<double> u_i = autovec.copy_col(i);
        Matriz<double> w_i = img * u_i;//Esto es un numero
        res += w_i[0][0] * u_i;
    }
    return res;
}

// Determina si una imagen es una cara o no
// Para lograrlo, compara la distancia entre la imagen normalizada y 
// la imagen transformada(sin reduccion de dimenciones)
// Si esta distancia es mayor que cierto limite, la imagen no es una cara
// img es la imagen que queremos saber si es una cara, es un vector fila
// datos_train son las imagenes de la base de datos
bool es_cara(const Matriz<double>& img, const Matriz<double>& media, int n, const Matriz<double>& datos_train, int k)
{
    Matriz<double> datos(img);
    // Normalizamos los datos de la imagen
    datos -= media;

    // Obtenemos autovectores para realizar la tc
    Matriz<double> autovec = pca_autovectores(datos_train, media, k);

    // Transformo la imagen al espacio de autocaras
    Matriz<double> datos_transformados = proyectar_cara(autovec, datos);

    // Veo la distancia entre la imagen y su proyeccion en el espacio de caras
    double e_d = norma_2_vec(datos.traspuesta() - datos_transformados);
    //double e_d = norma_1(datos.traspuesta() - datos_transformados);
    //double e_d = norma_infinito(datos.traspuesta() - datos_transformados);

    //Busco la mayor distancia entre una imagen de la base de train y su proyeccion de caras
    double e_max_train = 0.0;
    for(int i = 0;i < datos_train.filas();i++)
    {
        Matriz<double> dt = datos_train.copy_fil(i) - media;
        double dist = norma_2_vec(dt.traspuesta() - proyectar_cara(autovec, dt));
        //double e_d = norma_1(datos.traspuesta() - datos_transformados);
        //double e_d = norma_infinito(datos.traspuesta() - datos_transformados);

        if(dist > e_max_train)
            e_max_train = dist;
    }

    // Si la distancia entre la imagen desconocida y su proyeccion es menor al doble de la distancia maxima 
    // entre una imagen de la base y su proyeccion, decimos que es una cara
    std::cout<<"Distancia imagen: "<<e_d<<" | Distancia maxima: "<<e_max_train<<'\n';
    return (e_d < e_max_train);
}

int tp2(IOUtils &utilidad, info_archivo &info){

    //Seteamos el seed de random
    srandom(0);
    
    utilidad.leer_archivos_csv(info.in_train,info.in_test,info);
   
    //Cambio la cantidad de digitos con las que nos muestra los doubles en pantalla
    cout << fixed;std::setprecision(50);   
    //cout << info << endl;
    Matriz<double> train(utilidad.armar_base_entrenamiento(info));
    Matriz<double> test(utilidad.armar_casos_tests(info));

    // Obtenemos los sujetos de cada imagen de la base de train y test
    DiccNatANat s_train(utilidad.obtener_sujetos_train(info));
    vector<unsigned int> s_test(utilidad.obtener_sujetos_test(info));

    // Escribimos los autovalores de la matriz con la que hacemos el cambio de variables
    if (!info.experimentacion) {utilidad.escribir_autovalores(train, info, info.out_res);}

    // Vemos si la primer imagen de test es una cara o no
    if(info.m == ES_CARA)
    {
        Matriz<double> med = media(train);
        int n = train.filas();
        unsigned int norma_cara = info.k;
        for(int i = 0;i < test.filas();i++)
            std::cout<<es_cara(test.copy_fil(i), med, n, train, norma_cara)<<'\n';
        return 0;
    }

    clock_t total = 0;
    // Si usamos PCA o PCA_distancia, realizamos la transformacion de las bases
    if(info.m == PCA || info.m == PCA_distancia)
    { 
        clock_t conv = clock();

        Matriz<double> med = media(train);
        int n = train.filas();

        Matriz<double> autovec = pca_autovectores(train, med, info.alpha);
        // Aplico la transformacion dada por PCA sobre los datos de la base de train
        train = tc_matriz(autovec, train, med, n);  

        // Aplico la transformacion dada por PCA sobre los datos de la base de test 
        test = tc_matriz(autovec, test, med, n);    

        conv = (clock() - conv);
        total = conv;
    }
     
    clock_t clasif = clock();
    // Clasificamos las imagenes de test usando las de train y obtenemos mediciones
    Clasificador c(train, s_train);
    medidas_info r = c.clasificar_y_medir(test, s_test, info.k, info.con_distancia);
    clasif = ((clock() - clasif));
    total = total + clasif;

    utilidad.escribir_output(info.out_res, info, r.sujetos_predichos);

    // Escribimos las mediciones en un archivo

    utilidad.escribir_medidas(info, r, total);

    if (!info.experimentacion){
        //cout << "modo:  " << info.m << endl;
         //cout << "k: " << info.k << endl;
         //cout << "alpha: " << info.alpha << endl;
         //cout << "tamaño: " << info.size << endl;
         cout << r << endl;
     }

}


int main(int argc, char* argv[]){
    //Filtramos los datos de entrada
   
    IOUtils utilidad;
    info_archivo info;
    utilidad.parse(argc,argv, info);

    tp2(utilidad,info);
   

    return 0;
}
