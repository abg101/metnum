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
    unsigned int p;
    unsigned int nimgp;
    unsigned int total_imagenes_train;
    unsigned int k;
    unsigned int ntest;
    DiccDatostrainXSujeto imgs_a_considerar_x_sujeto;
    std::vector<caso_test> casos_a_testear;
};

//Modo de operacion
// dond dice _pesos en realidad iria _distancia, pero es tarde para cambiar eso
// kNN y kNN_pesos estan para debug
enum Modo{
    kNN = 0,
    kNN_pesos,
    PCA,
    PCA_pesos,
    ES_CARA
};

//Para mostrar/escribir la info de un archivo.in
std::ostream& operator<<(std::ostream& os, const info_archivo& obj)
{
    os<<'\n';
    os<<"Path de la base de datos: "<<obj.path_base<<'\n';
    os<<"Ancho de las imagenes: "<<obj.ancho_imagen<<'\n';
    os<<"Alto de las imagenes: "<<obj.alto_imagen<<'\n';
    os<<"Cantidad de sujetos: "<<obj.p<<'\n';
    os<<"Cantidad maxima de imagenes por sujeto: "<<obj.nimgp<<'\n';
    os<<"Cantidad de componentes principales(k): "<<obj.k<<'\n';
    os<<"Cantidad de imagenes a testear: "<<obj.ntest<<'\n';

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

// Leemos el archivo de input, el .in
void leer_archivo(std::string path, info_archivo& res)
{
    std::fstream fs;
    fs.open(path.c_str(),std::fstream::in);
    
    if(!fs.good())
    {
        std::cout<<"Fallo lectura archivo .in";
        return;
    }

    fs >> res.path_base;
    fs >> res.alto_imagen; 
    fs >> res.ancho_imagen;
    fs >> res.p;
    fs >> res.nimgp;
    fs >> res.k;

    res.total_imagenes_train = 0;
    for(unsigned int i = 0;i < res.p;i++)
    {
        datos_sujeto ds;
        unsigned int s;

        fs >> ds.path_imagenes;

        //Para saber el sujeto al que pertenecen las imagenes, vemos el path donde se entran dichas imagenes, por lo general de la forma "s[sujeto]/.."
        //Nos interesa extraer [sujeto de ese string]
        //Primero tomamos la parte del path que queremos, "s[sujeto]/", extraemos [sujeto] y lo convertimos a uint
        s = std::stoul(ds.path_imagenes.substr(1, ds.path_imagenes.find_first_of("/")-1));

        unsigned int j = 0;

        while(j < res.nimgp && fs.peek() != '\n')
        {
            unsigned int temp;
            fs >> temp;
            ds.imgs_entrenamiento.push_back(temp);
            j++;
            res.total_imagenes_train++;
        }
        res.imgs_a_considerar_x_sujeto[s] = ds;
    }
    fs >> res.ntest;
    res.casos_a_testear.resize(res.ntest);

    for(unsigned int i = 0;i < res.ntest;i++)
    {
        caso_test ct;
        fs >> ct.path_imagen;
        fs >> ct.sujeto;
        res.casos_a_testear[i] = ct;
    }

    fs.close();
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
Matriz<double> leer_imagen(std::string path)
{
    std::fstream fs;

    fs.open(path.c_str(),std::fstream::in);

    if(!fs.good())
    {
        std::cout<<"Fallo lectura de la imagen "<<path<<'\n';
        assert(1 == 2); //Hago esto para el programa termine de inmediato
        Matriz<double> res(1, 1, 1);
        return res;
    }
    // Vemos como esta formado el header de la imagen
    // Para ver como lo leemos

    // Guardamos la posicion original de lectura 
    int len = fs.tellg();
    std::string primer_linea;

    // Leo primer linea
    getline(fs, primer_linea);

    // Vuelvo la posicion de lectura hacia atras, antes de leer la primer linea
    fs.seekg(len, std::ios_base::beg);

    unsigned int alto = 0; 
    unsigned int ancho = 0;
    if(primer_linea.size() <= 3) //Si la primer linea es solo "P5", leemos la imagen de esta forma
    {
        fs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//ignoro la primer linea, contiene "P5", basicamente dice que formato es   
        if(fs.peek() == '#') 
            fs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//ignoro la segunda linea, contiene comentario   
         
        fs >> ancho;
        fs >> alto;
        
        //ignoro el final de la linea de ancho y alto
        fs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        //ignoro la cuarta linea, contiene el valor max de la imagen
        fs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    else//Si la primer linea es "P5" mas algunos argumentos, leemos la imagen de esta forma
    {
        char ignorar_caracter;
            
            // Ignoro los primeros dos caracteres que dicen P5
        fs>>ignorar_caracter;
        fs>>ignorar_caracter;
            
        // Leo tamaño de la imagen
        fs >> ancho;
        fs >> alto;

        // Ignoro el valor max de la imagen
        fs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');  
    }

    //Leo pixeles de la imagen, cada pixel es un valor entre 0 y 255
    Matriz<double> temp(ancho*alto, 1, 0);

    for(unsigned int i = 0;i < ancho*alto;i++)
    {
        unsigned char c;//Leo los char como bytes, por eso uso unsigned char
        fs >>std::noskipws>> c;//noskipsws indica que no se ignoren espacios en blanco, ya que estos son valores de un pixel
        
        //Si por algun motivo llegamos al final del archivo, corto el loop. No se si esto puede ocurrir normalmente o solo en caso de error
        if(fs.eof())       
            break;

        temp[i][0] = double(c);
    }
    fs.close();
        
    return temp;
}

// Devuelve una matriz con las imagenes de entrenamiento como vectores fila
Matriz<double> armar_base_entrenamiento(const info_archivo& ia)
{
    Matriz<double> res(ia.total_imagenes_train, ia.alto_imagen * ia.ancho_imagen, 0);
    int cant_img = 0;

    for(DiccDatostrainXSujeto::const_iterator it = ia.imgs_a_considerar_x_sujeto.cbegin();it != ia.imgs_a_considerar_x_sujeto.cend();++it)
    {
        for(unsigned int i : (it->second).imgs_entrenamiento)
        {
            std::string path_imagen = ia.path_base + (it->second).path_imagenes + std::to_string(i) + ".pgm";
            Matriz<double> img = leer_imagen(path_imagen);
            res.set_fil(cant_img, img);
            cant_img++;
        }
    }

    return res;
}

// Devuelve una matriz con las imagenes de test como vectores fila
Matriz<double> armar_casos_tests(const info_archivo& ia)
{
    Matriz<double> res(ia.ntest, ia.alto_imagen * ia.ancho_imagen, 0);
    int cant_img = 0;
    for(const caso_test& ct : ia.casos_a_testear)
    {
        res.set_fil(cant_img, leer_imagen(ct.path_imagen));
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
    Matriz<double> autoval_pca = pca_autovalores(datos, info.k);

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
    if (argc < 3)
    {
        std::cout<<"Parametros de entrada insuficientes"<<'\n';
        return 1;
    }
    
    std::string input(argv[1]);
    std::string output(argv[2]);
    std::string output_medidas(output + ".medidas");
    Modo m = PCA;
    unsigned int valor_knn = 1;
    bool con_pesos = false;

    // Vemos que metodo para clasificar vamos a usar
    // kNN, kNN con pesos, PCA + kNN o PCA + kNN con pesos
    // Ademas necesitamos el valor k de kNN
    if(argc >= 5)
    {
        std::string modo_s(argv[3]);
        std::string k_kNN(argv[4]);
    
        valor_knn = std::stoul(k_kNN);
        if(valor_knn == 0)
        {
            std::cout<<"ERROR: Valor k para kNN tiene que ser distinto de 0.\n";
            return 1;
        }

        if(modo_s == "kNN")
            m = kNN;
        else if(modo_s == "kNN_pesos")
        {
            m = kNN_pesos;
            con_pesos = true;
        }
        else if(modo_s == "PCA")
            m = PCA;
        else if(modo_s == "PCA_pesos")
        {
            m = PCA_pesos;
            con_pesos = true;
        }
        else if(modo_s == "ES_CARA")
            m = ES_CARA;
        else
        {
            std::cout<<"ERROR:Parametro de operacion incorrecto\n";
            return 1;
        }
    }

    // Si fue proporcionado, leemos el path del output de las medidas
    if(argc >= 6)
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

    //Si existe el archivo output, lo borramos
    std::ifstream test_out(output.c_str());
    if(test_out.good())
    {
      test_out.close();
      std::remove(output.c_str()); 
    }

    //Cargo los datos del archivo input
    info_archivo info;
    leer_archivo(input, info);  
    //Cambio la cantidad de digitos con las que nos muestra los doubles en pantalla
    std::cout<<std::fixed;std::setprecision(50);   
  
    Matriz<double> train(armar_base_entrenamiento(info));
    Matriz<double> test(armar_casos_tests(info));

    // Obtenemos los sujetos de cada imagen de la base de train y test
    DiccNatANat s_train(obtener_sujetos_train(info));
    std::vector<unsigned int> s_test(obtener_sujetos_test(info));
   
    // Escribimos los autovalores de la matriz con la que hacemos el cambio de variables
    escribir_autovalores(train, info, output);

    // Vemos si la primer imagen de test es una cara o no
    // Todavian no funciona bien ni esta terminado
    if(m == ES_CARA)
    {
        Matriz<double> med = media(train);
        int n = train.filas();
        unsigned int norma_cara = valor_knn;
        for(int i = 0;i < test.filas();i++)
            std::cout<<es_cara(test.copy_fil(i), med, n, train, norma_cara)<<'\n';
        return 0;
    }

    clock_t total = 0;
    // Si usamos PCA o PCA_pesos, realizamos la transformacion de las bases
    if(m == PCA || m == PCA_pesos)
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
    medidas_info r = c.clasificar_y_medir(test, s_test, valor_knn, con_pesos);
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
