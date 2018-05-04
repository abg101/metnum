#ifndef CLASIFICADOR_HPP
#define CLASIFICADOR_HPP
#include "matriz.hpp"
#include "normas_aux.hpp"
#include <vector>
#include <map>
#include <unordered_map>
#include <stdlib.h>
#include <algorithm>
#include <utility>

typedef std::unordered_map<unsigned int, unsigned int> DiccNatANat;
typedef std::map<unsigned int, double> DiccNatADouble;

struct medidas_info
{
    double hit_rate_general;
    double precision_general;
    double recall_general;
    DiccNatADouble hitrate_x_sujeto;
    DiccNatADouble precision_x_sujeto;
    DiccNatADouble recall_x_sujeto;
};

std::ostream& operator<<(std::ostream& os, medidas_info& obj)
{
    os<<"Hitrate general\n";
    os<<obj.hit_rate_general<<'\n';
    os<<"Precision general\n";
    os<<obj.precision_general<<'\n';
    os<<"Recall general\n";
    os<<obj.recall_general<<'\n';

    os<<"Hitrate por sujeto\n";
    for(DiccNatADouble::const_iterator it = obj.hitrate_x_sujeto.cbegin();it != obj.hitrate_x_sujeto.cend();++it)
        os<<it->first<<": "<<it->second<<'\n';
    
    os<<"Precision por sujeto\n";
    for(DiccNatADouble::const_iterator it = obj.precision_x_sujeto.cbegin();it != obj.precision_x_sujeto.cend();++it)
        os<<it->first<<": "<<it->second<<'\n';

    os<<"Recall por sujeto\n";
    for(DiccNatADouble::const_iterator it = obj.recall_x_sujeto.cbegin();it != obj.recall_x_sujeto.cend();++it)
        os<<it->first<<": "<<it->second<<'\n';

    return os;
}



class Clasificador
{
    public:
        Clasificador(const Matriz<double>&, const DiccNatANat&);

        std::vector<unsigned int> clasificar(unsigned int k, const Matriz<double>& casos_tests, bool kNN_peso) const; 
        medidas_info clasificar_y_medir(const Matriz<double>&, std::vector<unsigned int>, unsigned int k, bool kNN_peso) const;
        void asignar_base_de_datos(const Matriz<double>&, const DiccNatANat&);


    private:
        unsigned int kNN(unsigned int k, const Matriz<double>& caso_test) const;
        unsigned int kNN_con_peso(unsigned int k, const Matriz<double>& caso_test) const;

        Matriz<double> base_datos;
        DiccNatANat sujeto_x_imagen;
};

Clasificador::Clasificador(const Matriz<double>& datos, const DiccNatANat& sujetos):base_datos(datos),sujeto_x_imagen(sujetos){};

void Clasificador::asignar_base_de_datos(const Matriz<double>& datos, const DiccNatANat& sujetos)
{
    base_datos = datos;
    sujeto_x_imagen = sujetos;
};

// Dada una matriz de imagen casos_test, devuelve un vector con las predicciones de clases hecha por kNN
std::vector<unsigned int> Clasificador::clasificar(unsigned int k, const Matriz<double>& casos_tests, bool kNN_peso) const
{
    std::vector<unsigned int> res(casos_tests.filas(), 0);
    for(int i = 0;i < casos_tests.filas();i++)
    {
        Matriz<double> imagen_a_testear(casos_tests.copy_fil(i));

        if(!kNN_peso)
            res[i] = kNN(k, imagen_a_testear);
        else
            res[i] = kNN_con_peso(k, imagen_a_testear);
    }   
    return res;
};

// Clasifica los datos de test sobre la base de datos y devuelve un sumario con toda la info
// tests es la matriz con las imagenes a clasificar
// sujetos_tests contiene los sujetos a los que corresponde cada imagen de tests, lo usamos para medir como clasificamos
// k es el valor de vecinos a comparar en kNN
// kNN_peso es si se va a usar kNN_con_peso o kNN comun
medidas_info Clasificador::clasificar_y_medir(const Matriz<double>& tests, std::vector<unsigned int> sujetos_tests, unsigned int k, bool kNN_peso) const
{
    medidas_info info;

    unsigned int aciertos = 0;
    unsigned int total = tests.filas();

    DiccNatADouble total_x_cat;
    DiccNatADouble verdaderos_pos_cat;
    DiccNatADouble falsos_pos_cat;
    DiccNatADouble falsos_neg_cat;

    // Clasificamos las imagenes de tests
    std::vector<unsigned int> sujetos_predichos(clasificar(k, tests, kNN_peso));

    for(int i = 0; i < tests.filas();i++)
    {
        if(sujetos_predichos[i] == sujetos_tests[i])
        {
            aciertos++;

            if(verdaderos_pos_cat.count(sujetos_predichos[i]) == 0)
                verdaderos_pos_cat[sujetos_predichos[i]] = 0;

            verdaderos_pos_cat[sujetos_predichos[i]]++;
        }
        else
        {
            if(falsos_neg_cat.count(sujetos_tests[i]) == 0)
                falsos_neg_cat[sujetos_tests[i]] = 0;

            if(falsos_pos_cat.count(sujetos_predichos[i]) == 0)
                falsos_pos_cat[sujetos_predichos[i]] = 0;

            falsos_neg_cat[sujetos_tests[i]]++;
            falsos_pos_cat[sujetos_predichos[i]]++;
        }

        if(total_x_cat.count(sujetos_tests[i]) == 0)
            total_x_cat[sujetos_tests[i]] = 0;

        total_x_cat[sujetos_tests[i]]++;
    }

    // Calculo hitrate, precision y recall general
    info.hit_rate_general = double(aciertos)/double(total);

    info.precision_general = 0;
    for(DiccNatADouble::const_iterator i = verdaderos_pos_cat.cbegin();i != verdaderos_pos_cat.cend();++i)
        if(i->second > 0)
	        info.precision_general += (i->second)/(i->second + falsos_pos_cat[i->first]);

    info.precision_general /= double(total_x_cat.size());

    info.recall_general = 0;
    for(DiccNatADouble::const_iterator i = verdaderos_pos_cat.cbegin();i != verdaderos_pos_cat.cend();++i)
        if(i->second > 0)
	        info.recall_general += (i->second)/(i->second + falsos_neg_cat[i->first]);

    info.recall_general /= double(total_x_cat.size());

    // Calculo hitrate, precision y recall por categoria
    for(DiccNatADouble::const_iterator it = total_x_cat.cbegin();it != total_x_cat.cend();++it)
        info.hitrate_x_sujeto[it->first] = verdaderos_pos_cat[it->first] / it->second;        

    for(DiccNatADouble::const_iterator it = total_x_cat.cbegin();it != total_x_cat.cend();++it)
    {
        double v_pos = verdaderos_pos_cat[it->first];
        double f_pos = falsos_pos_cat[it->first];
        if(v_pos + f_pos > 0)
            info.precision_x_sujeto[it->first] = v_pos / (v_pos + f_pos);
        else
            info.precision_x_sujeto[it->first] = 0;
    }

    for(DiccNatADouble::const_iterator it = total_x_cat.cbegin();it != total_x_cat.cend();++it)
    {
        double v_pos = verdaderos_pos_cat[it->first];
        double f_neg = falsos_neg_cat[it->first];

        if(v_pos + f_neg > 0)
            info.recall_x_sujeto[it->first] = v_pos / (v_pos + f_neg);
        else
            info.recall_x_sujeto[it->first] = 0;
    }

    return info;
};
               	 
/*
 *
 * Implementaciones de kNN
 *
 */


// Metodo kNN canonico
// Primero busca los k vecinos mas cercanos usando norma2
// Luego realiza la votacion para ver a que clase corresponde el caso de test
// Falta agregar que ocurre en caso de empate
// Por ahora, en caso de empate, gana el que este primero en la lista de votos
unsigned int Clasificador::kNN(unsigned int k, const Matriz<double>& caso_test) const
{
    std::vector<double> min(k,std::numeric_limits<double>::max());
    std::vector<unsigned int> res(k, 0);
    Matriz<double> resta(caso_test);

    //Buscamos los k vecinos mas cercanos
    for(int i=0;i < base_datos.filas();i++)
    {
        Matriz<double> otra(base_datos.copy_fil(i));
        resta -= otra;
        
        double distancia = norma_2_vec(resta);
        unsigned int actual = i;

        for(unsigned int j=0;j < k;j++)
        {
            if(distancia < min[j])
            {
                std::swap(min[j], distancia);
                std::swap(res[j], actual);
            }
        } 
        resta = caso_test;
    }

    //Votamos para ver a cual sujeto corresponde la imagen
    //Primero vemos cuales son los sujetos y contamos los votos para cado uno
    DiccNatANat sujetos_votados;
    for(unsigned int i = 0;i < k;i++)
    {
        if(sujetos_votados.count(sujeto_x_imagen.at(res[i])) == 0)
            sujetos_votados[sujeto_x_imagen.at(res[i])] = 1;
        else 
            sujetos_votados[sujeto_x_imagen.at(res[i])] += 1;
    }

    //Vemos cual sujeto recibio la mayor cantidad de votos y lo devolvemos como resultado
    unsigned int max     = 0;
    unsigned int arg_max = 0;
    for(DiccNatANat::const_iterator it = sujetos_votados.cbegin(); it != sujetos_votados.cend();++it)
    {
        if(it->second > max)
        {
            max = it->second;
            arg_max = it->first;
        }
    }

    return arg_max;

};

// Metodo kNN con pesos en base a la distnacia
// Primero busca los k vecinos mas cercanos usando norma2
// Luego realiza la votacion para ver a que clase corresponde el caso de test, 
// tomando en cuenta la clase a la que pertenecen y la distancia con respecto al caso_test
// Falta agregar que ocurre en caso de empate
// Por ahora, en caso de empate, gana el que este primero en la lista de votos
unsigned int Clasificador::kNN_con_peso(unsigned int k, const Matriz<double>& caso_test) const
{
    std::vector<double> min(k,std::numeric_limits<double>::max());
    std::vector<unsigned int> res(k, 0);
    Matriz<double> resta(caso_test);

    //Buscamos los k vecinos mas cercanos
    for(int i=0;i < base_datos.filas();i++)
    {
        Matriz<double> otra(base_datos.copy_fil(i));
        resta -= otra;

        double distancia = norma_2_vec(resta);
        unsigned int actual = i;
        for(unsigned int j=0;j < k;j++)
        {
            if(distancia < min[j])
            {
                std::swap(min[j], distancia);
                std::swap(res[j], actual);
            }
        } 

        resta = caso_test;
    }

    //Votamos para ver a cual sujeto corresponde la imagen
    //Primero vemos cuales son los sujetos y contamos los votos, pesados por la distancia, para cado uno
    DiccNatADouble sujetos_votados;
    for(unsigned int i = 0;i < k;i++)
    {
        if(sujetos_votados.count(sujeto_x_imagen.at(res[i])) == 0)
            sujetos_votados[sujeto_x_imagen.at(res[i])] = (1.0/min[i]);
        else 
            sujetos_votados[sujeto_x_imagen.at(res[i])] += (1.0/min[i]);
    }

    //Vemos cual sujeto recibio la mayor cantidad de votos y lo devolvemos como resultado
    double max = 0;
    unsigned int arg_max = 0;
    for(DiccNatADouble::const_iterator it = sujetos_votados.cbegin(); it != sujetos_votados.cend();++it)
    {
        if(it->second > max)
        {
            max = it->second;
            arg_max = it->first;
        }
    }

    return arg_max;
};
#endif
