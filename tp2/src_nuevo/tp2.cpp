#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "matriz_aux.hpp"
#include "normas_aux.hpp"
#include <cstdio>
#include <limits>
#include "metodos_reduc.hpp"
#include <cmath>
#include <ctime>
#include "clasificador.hpp"
#include <algorithm>// max y transform
#include "ppmloader/ppmloader.h"

using namespace std;

const int CANT_POTENCIA = 10000;

struct datos_sujeto
{
    std::string path_imagenes;
    std::vector<unsigned int> imgs_entrenamiento;
};

struct caso_test
{
    std::string path_imagen;
    unsigned int sujeto;
};

typedef std::map<unsigned int, datos_sujeto> DiccDatostrainXSujeto;

struct info_archivo
{
    std::string path_base;
    unsigned int ancho_imagen;
    unsigned int alto_imagen;
    unsigned int cant_sujetos;
    unsigned int nimgp;
    unsigned int n_train;
    unsigned int n_test;
    unsigned int k;
    unsigned int alpha;
    DiccDatostrainXSujeto imgs_a_considerar_x_sujeto;
    std::vector<caso_test> casos_a_testear;
};

//Modo de operacion
// kNN y kNN_distancia estan para debug
enum Modo{
    kNN = 0,
    kNN_distancia,
    PCA,
    PCA_distancia,
    ES_CARA
};

//Para mostrar/escribir la info de un archivo.in
std::ostream& operator<<(std::ostream& os, const info_archivo& obj)
{
    os<<'\n';
    os<<"Path de la base de datos: "<<obj.path_base<<'\n';
    os<<"Ancho de las imagenes: "<<obj.ancho_imagen<<'\n';
    os<<"Alto de las imagenes: "<<obj.alto_imagen<<'\n';
    os<<"Cantidad de sujetos: "<<obj.cant_sujetos<<'\n';
    os<<"Cantidad de imagenes a entrenar: "<<obj.n_train<<'\n';
    os<<"Cantidad maxima de imagenes por sujeto: "<<obj.nimgp<<'\n';
    os<<"Cantidad de componentes principales(k): "<<obj.k<<'\n';
    os<<"Cantidad de imagenes a testear: "<<obj.n_test<<'\n';

    os<<"-------------------\n";
    os<<"Imagenes de la base\n";
    os<<"-------------------\n";

    for(DiccDatostrainXSujeto::const_iterator it = obj.imgs_a_considerar_x_sujeto.cbegin();it != obj.imgs_a_considerar_x_sujeto.cend();++it)
    {
        os<<(it->second).path_imagenes<<" ";
        for(unsigned int j : (it->second).imgs_entrenamiento)
            os<<j<<" ";
        os<<'\n';
    }

    os<<"-------------------\n";
    os<<"Imagenes a testear \n";
    os<<"-------------------\n";

    for(const caso_test& i : obj.casos_a_testear)
        os<<i.path_imagen<<" | Sujeto "<<i.sujeto<<'\n';

    return os;
}


void leer_archivos_csv(std::string path_1, std::string path_2, info_archivo& res){
    std::fstream fs1;
    fs1.open(path_1.c_str(),std::fstream::in);
    
    if(!fs1.good())
    {
        std::cout<<"Fallo lectura archivo 1 .csv";
        return;
    }

    std::fstream fs2;
    fs2.open(path_2.c_str(),std::fstream::in);
    
    if(!fs2.good())
    {
        std::cout<<"Fallo lectura archivo 2 .csv";
        return;
    }
    
    std::string temp1;
    std::string temp2;
    res.cant_sujetos = 0;
    res.nimgp = 0;
    res.n_train = 0;
    bool termine = false;
    for(unsigned int i = 0;i < 41;i++)
    {                
        datos_sujeto ds;
        unsigned int s = 0;
        unsigned int temp3 = 0;
        unsigned int aux_nimgp = 1;
    
        fs1 >> temp1;
        fs1 >> temp2;
    
        res.n_train++;
   
        unsigned int num_imagen = std::stoul(temp1.substr(temp1.find_last_of("/")+1, temp1.find_last_of(".")-1));

        //Para saber el path_imagenes al que pertenecen las imagenes, vemos el path donde se entran dichas imagenes, por lo gral de la forma "../data/ImagenesCaras/s{0}/{1}.pgm,"
        // o "../data/ImagenesCarasRed/s{0}/{1}.pgm,"
        temp1 = temp1.substr(0, temp1.find_last_of("/"));
        ds.path_imagenes = temp1.substr(temp1.find_last_of("/")+1,temp1.length()+1) + "/";
        
        int len; 
        while(temp3 == s && !(fs1.eof())){
            ds.imgs_entrenamiento.push_back(num_imagen);
            s = std::stoul(temp2.substr(0, temp2.find_first_of(",")));
            
            //posicion antes de leer la linea
            len = fs1.tellg();
            
            fs1 >> temp1;
            fs1 >> temp2;            

            res.n_train++;
            aux_nimgp++;

            num_imagen = std::stoul(temp1.substr(temp1.find_last_of("/")+1, temp1.find_last_of(".")-1));
            temp3 = std::stoul(temp2.substr(0, temp2.find_first_of(",")));
            //para no restar en la ultima iteracion
            if(fs1.eof()){  
                termine = true;
                ds.imgs_entrenamiento.push_back(num_imagen);
            }
        }
        // Vuelvo la posicion de lectura hacia atras, antes de leer la primer linea
        fs1.seekg(len, std::ios_base::beg);
        
        if(!(termine)){
            res.n_train--;
            aux_nimgp--;
        }

        res.nimgp = std::max(res.nimgp,aux_nimgp);
        res.imgs_a_considerar_x_sujeto[s] = ds;
        res.cant_sujetos++;
    }    
    fs1.close();
    
    res.casos_a_testear.resize(41 * res.nimgp); //lo defino lo mas grande posible
	res.n_test = 0;
    unsigned int i = 0;
    while(i < res.casos_a_testear.size() && !(fs2.eof()))
    {
        caso_test ct;
        fs2 >> temp1;
        ct.path_imagen = temp1.substr(0, temp1.find_last_of(","));
 
        fs2 >> temp2;
        ct.sujeto = std::stoul(temp2.substr(0, temp2.find_first_of(",")));
 
        res.casos_a_testear[i] = ct;
        res.n_test++;
		i++;
    }
    res.casos_a_testear.resize(res.n_test);

    fs2.close();
}




// Lee una imagen de formato .pgm
// Esta imagenes se las puede interpretar como archivos de texto armados de las siguientes maneras:
//
// P5
// [Comentario opcional, empieza con #]
// [Ancho] [Alto]
// [Valor max entre los pixeles]
// [Datos, todos seguidos, sin interrupcion, cada pixel es un byte(osea un char)]
//
// Alternativamente
//
// P5 [Ancho] [Alto] [Valor max]
// [Datos, idem al anterior]
//
Matriz<double> leer_imagen(std::string path, int ancho, int alto)
{
    
    uchar* imagen;
    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
    LoadPPMFile(&imagen, &ancho, &alto, &pt, path.c_str());

    //Leo pixeles de la imagen, cada pixel es un valor entre 0 y 255
    Matriz<double> temp(ancho*alto, 1, 0);

    for(unsigned int i = 0;i < ancho*alto;i++)
    {

        temp[i][0] = double(imagen[i]);
    }
        
    return temp;
}


// Devuelve una matriz con las imagenes de entrenamiento como vectores fila
Matriz<double> armar_base_entrenamiento(const info_archivo& ia)
{
    Matriz<double> res(ia.n_train, ia.alto_imagen * ia.ancho_imagen, 0);
    int cant_img = 0;

    for(DiccDatostrainXSujeto::const_iterator it = ia.imgs_a_considerar_x_sujeto.cbegin();it != ia.imgs_a_considerar_x_sujeto.cend();++it)
    {
        for(unsigned int i : (it->second).imgs_entrenamiento)
        {
            std::string path_imagen = ia.path_base + (it->second).path_imagenes + std::to_string(i) + ".pgm";
            Matriz<double> img = leer_imagen(path_imagen,ia.ancho_imagen, ia.alto_imagen);
            res.set_fil(cant_img, img);
            cant_img++;
        }
    }

    return res;
}

// Devuelve una matriz con las imagenes de test como vectores fila
Matriz<double> armar_casos_tests(const info_archivo& ia)
{
    Matriz<double> res(ia.n_test, ia.alto_imagen * ia.ancho_imagen, 0);
    int cant_img = 0;
    for(const caso_test& ct : ia.casos_a_testear)
    {
        res.set_fil(cant_img, leer_imagen(ct.path_imagen, ia.ancho_imagen, ia.alto_imagen));
        cant_img++;
    }

    return res;
}

//Devuelve un diccionario tal que para cada imagen de train, devuelve su sujeto.
DiccNatANat obtener_sujetos_train(const info_archivo& ia)
{
    DiccNatANat res;
    unsigned int cant_img = 0;
    for(DiccDatostrainXSujeto::const_iterator it = ia.imgs_a_considerar_x_sujeto.cbegin();it != ia.imgs_a_considerar_x_sujeto.cend();++it)
        for(unsigned int i = 0;i < (it->second).imgs_entrenamiento.size();i++)
        {
            res[cant_img] = it->first;
            cant_img++;
        }

    return res;
}

//Devuelve un vector tal que para cada imagen de test, devuelve su sujeto.
std::vector<unsigned int> obtener_sujetos_test(const info_archivo& ia)
{
    std::vector<unsigned int> res;
    for(const caso_test& ct : ia.casos_a_testear)
        res.push_back(ct.sujeto);

    return res;
}

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

// Escribe en el archivo de output los autovalores de la matriz de covarianza de datos
void escribir_autovalores(const Matriz<double>& datos, const info_archivo& info,const std::string& output)
{
    Matriz<double> autoval_pca = pca_autovalores(datos, info.alpha);

    std::fstream f;
    f.precision(10);
    f.open(output.c_str(),std::fstream::out | std::fstream::app);
    
    if(!f.good())
    {
        std::cout<<"Fallo al escribir autovalores\n";
        assert(false); //Hago esto para el programa termine de inmediato
    }

    for(int i = 0;i < autoval_pca.filas();i++)
        f<<std::sqrt(autoval_pca[i][0])<<'\n';

    f.close();
}

int main(int argc, char* argv[]){
    //Filtramos los datos de entrada
    if (argc < 9)
    {
        std::cout<<"Parametros de entrada insuficientes"<<'\n';
        return 1;
    }


    std::string in_train(argv[4]);
    std::string in_test(argv[6]);
    std::string out_res(argv[8]);
    std::string output_medidas(out_res + ".medidas");

    //Cargo los datos del archivo input
    info_archivo info;

    Modo m = kNN;
    std::string mod_s(argv[2]);
    unsigned int modo_s = std::stoul(mod_s);

    bool con_distancia = false;

    if(modo_s == 1) //"kNN"
        m = kNN;
    else if(modo_s == 2) //"PCA" 
        m = PCA;
    else if(modo_s == 3) //"kNN_distancia"
    {
        m = kNN_distancia;
        con_distancia = true;
    }
    else if(modo_s == 4)//"PCA_distancia"
    {
        m = PCA_distancia;
        con_distancia = true;
    }
    else if(modo_s == 5)//"ES_CARA"
        m = ES_CARA;
    else
    {
        std::cout<<"ERROR:Parametro de operacion incorrecto\n";
        return 1;
    }

    // Vemos que metodo para clasificar vamos a usar
    // kNN, kNN con distancia, PCA + kNN o PCA + kNN con distancia
    // Ademas necesitamos el valor k de kNN
    info.k = 1;
    if(argc >= 10)
    {
        std::string k_kNN(argv[9]); //donde vaya a estar el parámetro para el K de kNN
        info.k = std::stoul(k_kNN);

        if(info.k == 0)
        {
            std::cout<<"ERROR: Valor k para kNN tiene que ser distinto de 0.\n";
            return 1;
        }


    }

    info.alpha = 15;

    if(argc >= 11)
    {
        std::string alpha(argv[10]); //donde vaya a estar el parámetro para el K de kNN
        info.alpha = std::stoul(alpha);

        if(info.alpha == 0)
        {
            std::cout<<"ERROR: Valor alpha para PCA tiene que ser distinto de 0.\n";
            return 1;
        }


    }   
    
    // Si fue proporcionado, leemos el path del out_res de las medidas
    if(argc >= 13)
    {
        output_medidas = std::string(argv[5]);
        std::ifstream test_out(output_medidas.c_str());
        if(test_out.good())
        {
            test_out.close();
            std::remove(output_medidas.c_str()); 
        }
    }
    
    //Seteamos el seed de random
    srandom(0);

    //Si existe el archivo out_res, lo borramos
    std::ifstream test_out(out_res.c_str());
    if(test_out.good())
    {
      test_out.close();
      std::remove(out_res.c_str()); 
    }

    info.path_base = "data/ImagenesCaras/";
    info.alto_imagen = 112;
    info.ancho_imagen = 92;
    if(argc >= 12){
		std::string tipo(argv[11]);
		std::transform(tipo.begin(), tipo.end(), tipo.begin(), ::tolower);
        if(tipo == "big"){
            info.path_base = "data/ImagenesCaras/"; 
            info.alto_imagen = 112; 
            info.ancho_imagen = 92;
        }else if(tipo == "red"){
            info.path_base = "data/ImagenesCarasRed/";
            info.alto_imagen = 28; 
            info.ancho_imagen = 23;
        }else{
            std::cout<<"ERROR:Parametro de operacion incorrecto\n";
            return 1;
        }
    }

    leer_archivos_csv(in_train,in_test,info);
   
    //Cambio la cantidad de digitos con las que nos muestra los doubles en pantalla
    std::cout<<std::fixed;std::setprecision(50);   
  
    Matriz<double> train(armar_base_entrenamiento(info));
    Matriz<double> test(armar_casos_tests(info));

    // Obtenemos los sujetos de cada imagen de la base de train y test
    DiccNatANat s_train(obtener_sujetos_train(info));
    std::vector<unsigned int> s_test(obtener_sujetos_test(info));

    // Escribimos los autovalores de la matriz con la que hacemos el cambio de variables
    escribir_autovalores(train, info, out_res);

    // Vemos si la primer imagen de test es una cara o no
    if(m == ES_CARA)
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
    if(m == PCA || m == PCA_distancia)
    { 
        clock_t conv = clock();

        Matriz<double> med = media(train);
        int n = train.filas();

        Matriz<double> autovec = pca_autovectores(train, med, info.k);
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
    medidas_info r = c.clasificar_y_medir(test, s_test, info.k, con_distancia);
    clasif = ((clock() - clasif));
    total = total + clasif;

    // Escribimos las mediciones en un archivo
    std::fstream f;
    f.open(output_medidas.c_str(), std::fstream::out);
    if(!f.good())
        std::cout<<"Error: No se pudo escribir el archivo con las medidas tomadas\n";
    f<<std::setprecision(10);
    f<<"Tiempo(en ticks):\n"<<total<<'\n';
    f<<r;
    f.close();

    return 0;
}
