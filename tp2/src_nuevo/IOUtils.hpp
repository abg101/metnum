#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "ppmloader/ppmloader.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <limits>
#include <algorithm>
#include "preprocesamiento.hpp"
#include <getopt.h>

using namespace std;

struct datos_sujeto
{
    std::string path_imagenes;
    std::vector<unsigned int> imgs_entrenamiento;

    datos_sujeto(){
        path_imagenes = "";
        imgs_entrenamiento = vector<unsigned int> ();
    }
};

struct caso_test
{
    std::string path_imagen;
    unsigned int sujeto;
};

typedef std::map<unsigned int, datos_sujeto> DiccDatostrainXSujeto;

//Modo de operacion
// kNN y kNN_distancia estan para debug
enum Modo{
    kNN = 0,
    kNN_distancia,
    PCA,
    PCA_distancia,
    ES_CARA
};

struct info_archivo
{
    string path_base;
    string in_train;
    string in_test;
    string out_res;
    string output_medidas;
    Modo m;
    unsigned int ancho_imagen;
    unsigned int alto_imagen;
    unsigned int cant_sujetos;
    unsigned int n_train;
    unsigned int n_test;

    string size;
    bool experimentacion;
    bool con_distancia;

    unsigned int k;
    unsigned int alpha;
    DiccDatostrainXSujeto imgs_a_considerar_x_sujeto;
    vector<caso_test> casos_a_testear;
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
    os<<"Cantidad de componentes principales(k): "<<obj.alpha<<'\n';
    os<<"Cantidad de imagenes a testear: "<<obj.n_test<<'\n';
    os<<"path in train: "<<obj.in_train<<'\n';
    os<<"path in test: "<<obj.in_test<<'\n';
    os<<"mdeiciones: "<<obj.output_medidas<<'\n';
    os<<"k de knn: "<<obj.k<<'\n';
    os<<"m "<<obj.m<<'\n';
    os<<"size "<<obj.size<<'\n';
    os<<"experimentacion "<< obj.experimentacion <<'\n';
    os<<"Con distnacia "<<obj.con_distancia<<'\n';

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

class IOUtils {
public:
	IOUtils();
	~IOUtils();
	int parse(int arc, char** argv, info_archivo& info);
	void leer_archivos_csv(string path_1, string path_2, info_archivo& info);
	Matriz<double> leer_imagen(std::string path, int ancho, int alto);
	Matriz<double> armar_base_entrenamiento(const info_archivo& ia);
	DiccNatANat obtener_sujetos_train(const info_archivo& ia);
	std::vector<unsigned int> obtener_sujetos_test(const info_archivo& ia);
	Matriz<double> armar_casos_tests(const info_archivo& ia);
	void escribir_autovalores(const Matriz<double>& datos, const info_archivo& info,const std::string& output);
	void escribir_output(string &out_res, info_archivo &info, vector<unsigned int> &sujetos_predichos);
	void escribir_medidas(info_archivo &info, medidas_info &r, clock_t total);
};

IOUtils::IOUtils(){
}

IOUtils::~IOUtils(){
}


int IOUtils::parse(int argc, char** argv, info_archivo& info){

    if (argc < 9)
    {
        std::cout<<"Parametros de entrada insuficientes"<<'\n';
        return 1;
    }

    //Cargo los datos del archivo input
    info.k = 1;
    info.alpha = 15;
    info.m = kNN;
    int modo_s;
    info.size = "big";
    info.experimentacion = false;

   	char option;
    while ((option = getopt(argc, argv, "m:i:q:o:k:a:t:r:e")) != -1) {

        switch (option) {
            case 'm':{
            	    info.con_distancia = false;
            	    modo_s = stoi(optarg);
    				if(modo_s == 0) //"kNN"
                         info.m = kNN;
                     else if(modo_s == 1) //"PCA" 
                         info.m = PCA;
                     else if(modo_s == 2) //"kNN_distancia"
                     {
                         info.m = kNN_distancia;
                         info.con_distancia = true;
                     }
                     else if(modo_s == 3)//"PCA_distancia"
                     {
                         info.m = PCA_distancia;
                         info.con_distancia = true;
                     }
                     else if(modo_s == 4)//"ES_CARA"
                         info.m = ES_CARA;
                     else
                    {
                         std::cout<<"ERROR:Parametro de operacion incorrecto\n";
                         return 1;
                     }
                     break;
            }
            case 'i':{
            	info.in_train = optarg;
            	break;
            }
            case 'q':{
            	info.in_test = optarg;
            	break;
            }
            case 'o':{
            	info.out_res = optarg;
            	info.output_medidas = info.out_res + ".medidas";
            	break;
            }
            case 'k':{
            	info.k = stoi(optarg);

        		if(info.k == 0)
        		{
            		std::cout<<"ERROR: Valor k para kNN tiene que ser distinto de 0.\n";
            		return 1;
       			 }
            	break;
            }
            case 'a':{
            	info.alpha = stoi(optarg);
            	if(info.alpha == 0)
        		{
           			std::cout<<"ERROR: Valor alpha para PCA tiene que ser distinto de 0.\n";
            		return 1;
        		}
            	break;
            }         
            case 't':{
            	info.size = optarg;
            	break;
            }
            case 'r':{
            	info.output_medidas = optarg;
            	break;
            }
            case 'e':{
            	info.experimentacion = true;
            	break;
            }

            default: { // si las opciones son inválidas
           
                exit(1);
                break;
            }
        }

    }


    // Vemos que metodo para clasificar vamos a usar
    // kNN, kNN con distancia, PCA + kNN o PCA + kNN con distancia   
    
    // Si fue proporcionado, leemos el path del out_res de las medidas

    std::ifstream test_output_medida(info.output_medidas.c_str());
    if(test_output_medida.good())
    {
        test_output_medida.close();
        std::remove(info.output_medidas.c_str()); 
    }


    //Si existe el archivo out_res, lo borramos
    std::ifstream test_out_res(info.out_res.c_str());
    if(test_out_res.good())
    {
      (test_out_res).close();
      std::remove(info.out_res.c_str()); 
    }

    info.path_base = "data/ImagenesCaras/";
    info.alto_imagen = 112;
    info.ancho_imagen = 92;

		//std::transform(size.begin(), size.end(), size.begin(), ::tolower);
        if(info.size == "big"){
            info.path_base = "data/ImagenesCaras/"; 
            info.alto_imagen = 112; 
            info.ancho_imagen = 92;
        }else if(info.size == "red"){
            info.path_base = "data/ImagenesCarasRed/";
            info.alto_imagen = 28; 
            info.ancho_imagen = 23;
        }else{
            std::cout<<"ERROR:Parametro de operacion incorrecto\n";
            return 1;
        }

}    

void IOUtils::leer_archivos_csv(string path_1, string path_2, info_archivo& info){
    fstream fs1;
    fs1.open(path_1.c_str(),std::fstream::in);
    
    if(!fs1.good())
    {
        cout<<"Fallo lectura archivo 1 .csv";
        return;
    }

    fstream fs2;
    fs2.open(path_2.c_str(),std::fstream::in);
    
    if(!fs2.good())
    {
        std::cout<<"Fallo lectura archivo 2 .csv";
        return;
    }
    ///cout << "hola" << endl;

    string path;
    string id;
    info.cant_sujetos = 0;
    info.n_train = 0;


    while(fs1 >> path)
    {   
        fs1 >> id;
        datos_sujeto ds;
        unsigned int s = 0;
      
        unsigned int num_imagen = stoi(path.substr(path.find_last_of("/")+1, path.find_last_of(".")-1));
       
        //Para saber el path_imagenes al que pertenecen las imagenes, vemos el path donde se entran dichas imagenes, por lo gral de la forma "../data/ImagenesCaras/s{0}/{1}.pgm,"
        // o "../data/ImagenesCarasRed/s{0}/{1}.pgm,"
        path = path.substr(0, path.find_last_of("/"));
        s = stoi(id.substr(0, id.find_first_of(",")));

        //res.nimgp = smax(res.nimgp,aux_nimgp);
        info.imgs_a_considerar_x_sujeto[s].path_imagenes = path.substr(path.find_last_of("/")+1,path.length()+1) + "/";
        info.imgs_a_considerar_x_sujeto[s].imgs_entrenamiento.push_back(num_imagen);
        info.cant_sujetos++;
        info.n_train++;

    }    
    fs1.close();
    

    ////////////
    //info.casos_a_testear.resize(41 * res.nimgp); //lo defino lo mas grande posible
    info.casos_a_testear.reserve(41*10);
	info.n_test = 0;
    //unsigned int i = 0;
    while(fs2 >> path)
    {
        caso_test ct;
        fs2 >> id;
        ct.path_imagen = path.substr(0, path.find_last_of(","));
        ct.sujeto = stoi(id.substr(0, id.find_first_of(",")));
 
        info.casos_a_testear.push_back(ct);
        info.n_test++;;
    }

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
Matriz<double> IOUtils::leer_imagen(std::string path, int ancho, int alto)
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

    delete [] imagen;    
    return temp;
}


// Devuelve una matriz con las imagenes de entrenamiento como vectores fila
Matriz<double> IOUtils::armar_base_entrenamiento(const info_archivo& ia)
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
Matriz<double> IOUtils::armar_casos_tests(const info_archivo& ia)
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
DiccNatANat IOUtils::obtener_sujetos_train(const info_archivo& ia)
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
std::vector<unsigned int> IOUtils::obtener_sujetos_test(const info_archivo& ia)
{
    std::vector<unsigned int> res;
    for(const caso_test& ct : ia.casos_a_testear)
        res.push_back(ct.sujeto);

    return res;
}

// Escribe en el archivo de output los autovalores de la matriz de covarianza de datos
void IOUtils::escribir_autovalores(const Matriz<double>& datos, const info_archivo& info,const std::string& output)
{
    string filename = info.out_res+".autovalores";
    Matriz<double> autoval_pca = pca_autovalores(datos, info.alpha);

    std::ifstream test_out_autovalroes(filename.c_str());
    if(test_out_autovalroes.good())
    {
      (test_out_autovalroes).close();
      std::remove(filename.c_str()); 
    }

    std::fstream f;
    f.precision(10);
    f.open(filename.c_str(),std::fstream::out | std::fstream::app);
    
    if(!f.good())
    {
        std::cout<<"Fallo al escribir autovalores\n";
        assert(false); //Hago esto para el programa termine de inmediato
    }

    for(int i = 0;i < autoval_pca.filas();i++)
        f<<std::sqrt(autoval_pca[i][0])<<'\n';

    f.close();
}

void IOUtils::escribir_output(string &out_res, info_archivo &info, vector<unsigned int> &sujetos_predichos){

    ofstream file;
    file.open(out_res.c_str());
    for (int i = 0; i < sujetos_predichos.size(); ++i)
    {
        file << (info.casos_a_testear[i]).path_imagen << ", " << sujetos_predichos[i] << "," << endl;
    }
    file.close();
}

void IOUtils::escribir_medidas(info_archivo &info, medidas_info &r, clock_t total){

    fstream f;
    f.open((info.output_medidas).c_str(), std::fstream::out);
    if(!f.good())
        cout<<"Error: No se pudo escribir el archivo con las medidas tomadas\n";
    f<<std::setprecision(10);
    f<<"Tiempo(en ticks):\n"<<total<<'\n';
    f<<r;
    f.close();
}


#endif