#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include "matriz.hpp"
#include "matriz_aux.hpp"
#include <map>
#include <unordered_map>
#include <time.h>

struct Partido
{
    unsigned int fecha;
    unsigned int equipo_i;
    unsigned int goles_i;
    unsigned int equipo_j;
    unsigned int goles_j;
};

struct InfoArchivo
{
	unsigned int cant_equipos;
	unsigned int cant_partidos_total;
    std::vector<Partido> info_partidos;
};

enum Modo
{
    CMM_EG = 0,
    CMM_CL,
	WP
};

//std::map es un diccionario que respeta el orden de las claves(es un arbol). El orden sirve para iterar por fecha.
//std::unordered_map es un diccionario que no respeta el orden de las claves(tabla de hash).

//Renombre de Diccionario de Partidos ordenados por Fecha
typedef std::map<unsigned int, std::vector<Partido> > DiccPartidosXFecha;

//Renombre de Diccionario de Victorias por Equipo
typedef std::unordered_map<unsigned int, unsigned int> DiccEquipoVictorias;

//Renombre de Diccionario de Derrotas por Equipo
typedef std::unordered_map<unsigned int, unsigned int> DiccEquipoDerrotas;

//Renombre de Diccionario de Posicion por Equipo
typedef std::map<unsigned int, unsigned int> DiccEquipoAPos;

//Para mostrar/escribir la info de un partido
std::ostream& operator<<(std::ostream& os, const Partido& p)
{
    os<<"Fecha: "<<p.fecha<<" | "<<"Equipo i: "<<p.equipo_i<<" | "<<"Goles de i: "<<p.goles_i<<" | "<<"Equipo j: "<<p.equipo_j<<" | "<<"Goles de j: "<<p.goles_j<<'\n';
    return os;
}

//Para armar un partido a partir de un archivo de entrada
std::istream& operator>>(std::istream& is, Partido& p)
{
    is>>p.fecha;
    is>>p.equipo_i;
    is>>p.goles_i;
    is>>p.equipo_j;
    is>>p.goles_j;
    return is;
}

//Para mostrar/escribir la info de un archivo de entrada (para debug)
std::ostream& operator<<(std::ostream& os, const InfoArchivo& obj)
{
    os<<'\n';
    os<<"Cantidad de equipos: "<<obj.cant_equipos<<'\n';
    os<<"Cantidad de partidos jugados en total: "<<obj.cant_partidos_total<<'\n';
    os<<"Partidos jugados: "<<'\n';
    for(const Partido& p : obj.info_partidos)
        os<<p;
    os<<'\n';
	return os;
}

//Carga info del archivo de entrada InfoArchivo leer_archivo(std::string path)
InfoArchivo leer_archivo(std::string path)
{
    InfoArchivo res;

    std::fstream fs;
    fs.open(path.c_str(), std::fstream::in);
    if(fs.good() && fs.is_open())
    {
        fs>>res.cant_equipos;
        fs>>res.cant_partidos_total;

        //info_partidos es un vector vacio, por lo que hay que cambiar su tamaño mediante resize
        res.info_partidos.resize(res.cant_partidos_total);

        for(unsigned int i = 0;i < res.cant_partidos_total;i++)
            fs>>res.info_partidos[i];

        fs.close();
    }
    else
        std::cout<<"Error al leer archivo de entrada\n";

    return res;
}

//Escribimos el output como pide el enunciado
//Asume que datos es un vector columna que contiene el puntaje obtenido de cada equipo
void escribir_output(const Matriz<double>& res, std::string path)
{
    std::fstream fs;
    fs.open(path.c_str(), std::fstream::out);
    if(fs.good() && fs.is_open())
    {
        for(int i = 0;i < res.filas();i++)
            fs<<res[i][0]<<'\n';
        fs.close();
    }
    else
        std::cout<<"Error al escribir el archivo de salida\n";
}

//Los numeros que identifican a los equipos no siempre empiezan en 1 y no son contiguos
// La idea de esta funcion es crear un mapeo de cada equipo a un indice, que empiece en 0 y sean contiguos
// Esto se va a usar para armar la matriz, por eso empieza en 0 y no en 1
DiccEquipoAPos posicion_por_equipo(const DiccPartidosXFecha& ip)
{
    DiccEquipoAPos res;
    unsigned int i = 0;

    //Vamos a mapear primero todos los equipos que aparecen en equipo_i y luego los de equipo_j
    //Hacemos esto para respetar el orden usado en otras funciones

    for(DiccPartidosXFecha::const_iterator it = ip.cbegin();it != ip.cend(); ++it)
    {
        for(const Partido& p : it->second)
        {
            if(res.count(p.equipo_i) == 0) //Verifica que no esta mapeado
            {
                res[p.equipo_i] = i;
                i++;
            }
        }
    }

    for(DiccPartidosXFecha::const_iterator it = ip.cbegin();it != ip.cend(); ++it)
    {
        for(const Partido& p : it->second)
        {
            if(res.count(p.equipo_j) == 0) //Verifica que no esta mapeado
            {
                res[p.equipo_j] = i;
                i++;
            }
        }
    }
    return res;
}

//Devuelve un diccionario de partidos ordenados por fecha
DiccPartidosXFecha dividir_por_fecha(const InfoArchivo& a)
{
    DiccPartidosXFecha res;
    for(const Partido& p : a.info_partidos)
    {
        if(res.count(p.fecha) == 0)
            res[p.fecha] = std::vector<Partido>();

        res[p.fecha].push_back(p);
    }
    return res;
}

//Devuelve una lista con el numero de victorias lograda por cada equipo en el rango de fechas indicado.
DiccEquipoVictorias victorias_equipos(const DiccPartidosXFecha& ip)
{
    DiccEquipoVictorias res;
    for(DiccPartidosXFecha::const_iterator it = ip.cbegin();it != ip.cend();++it)
    {
        //Itero sobre los partidos de la fecha
        for(const Partido& p : it->second) 
        {
            if(p.goles_i > p.goles_j)
            {
                if(res.count(p.equipo_i) == 0) //Si no esta en el dicc, lo agrego
                    res[p.equipo_i] = 1;
                else
                    res[p.equipo_i]++;
            }
            else if(p.goles_j > p.goles_i)
            {
                if(res.count(p.equipo_j) == 0) //Si no esta en el dicc, lo agrego
                    res[p.equipo_j] = 1;
                else
                    res[p.equipo_j]++;
            }
        }

    }

    return res;
}

//Devuelve un diccionario con el numero de derrotas lograda por cada equipo
DiccEquipoDerrotas derrotas_equipos(const DiccPartidosXFecha& ip)
{
    DiccEquipoDerrotas res; 

    for(DiccPartidosXFecha::const_iterator it = ip.cbegin();it != ip.cend();++it)
    {
        //Itero sobre los partidos de la fecha it
        for(const Partido& p : it->second)
        {
            if(p.goles_i < p.goles_j)
            {
                if(res.count(p.equipo_i) == 0) //Si no esta en el dicc, lo agrego
                    res[p.equipo_i] = 1;
                else
                    res[p.equipo_i]++;
            }
            else if(p.goles_j < p.goles_i)
            {
                if(res.count(p.equipo_j) == 0) //Si no esta en el dicc, lo agrego
                    res[p.equipo_j] = 1;
                else
                    res[p.equipo_j]++;
            }

        }
    }

    return res;
}

//Devuelve una matriz entera donde cada posicion representa a n_ij
Matriz<unsigned int> partidos_entre_equipos(const InfoArchivo& ia, const DiccPartidosXFecha& ip,const DiccEquipoAPos& ep)
{
    Matriz<unsigned int> res(ia.cant_equipos, ia.cant_equipos, 0);

    for(DiccPartidosXFecha::const_iterator it = ip.cbegin();it != ip.cend();++it)
    {
        for(const Partido& p : it->second)
        {
            //Incremento n_ij 
            res[ep.at(p.equipo_i)][ep.at(p.equipo_j)]++;
            res[ep.at(p.equipo_j)][ep.at(p.equipo_i)]++;
            //Incremento n_i
            res[ep.at(p.equipo_i)][ep.at(p.equipo_i)]++;
            //Incremento n_j
            res[ep.at(p.equipo_j)][ep.at(p.equipo_j)]++;
        }
    }
    return res;
}

//Arma el vector columna b del sistema Cr=b
Matriz<double> armar_vector_b(const InfoArchivo& ia, const DiccPartidosXFecha& ip, const DiccEquipoAPos& ep)
{
    Matriz<double> res(ia.cant_equipos, 1, 0);

    DiccEquipoVictorias v(victorias_equipos(ip));
    DiccEquipoDerrotas d(derrotas_equipos(ip));

    //it->first es el equipo, it->second es la posicion asignada al equipo
    for(DiccEquipoAPos::const_iterator it = ep.cbegin();it != ep.cend();++it)
        res[it->second][0] = 1 + double(int(v[it->first]) - int(d[it->first]))/2.0;

    return res;
}

//Arma la matriz C del sistema Cr=b
Matriz<double> armar_matriz(const InfoArchivo& ia, const DiccPartidosXFecha& ip, const DiccEquipoAPos& ep)
{
    Matriz<double> res(ia.cant_equipos, ia.cant_equipos, 0);
    
    //Armo una matriz con los n_i y los n_ij
    Matriz<unsigned int> datos_part(partidos_entre_equipos(ia, ip, ep)); 

    //Asignamos los coeficientes pedidos por CMM
    for(DiccEquipoAPos::const_iterator it = ep.cbegin();it != ep.cend();++it)
    {
        for(DiccEquipoAPos::const_iterator it2 = ep.cbegin();it2 != ep.cend();++it2)
        {
            unsigned int i = it->second;
            unsigned int j = it2->second;

            if(i == j)
                res[i][j] = double(2 + datos_part[i][i]);
            else
            {
                res[i][j] = -1.0 * double(datos_part[i][j]);
                res[j][i] = -1.0 * double(datos_part[i][j]);
            }
        }
    }

    return res;
}

//Arma un vector con el WP de cada equipo en cada posicion
Matriz<double> calcular_wp(const InfoArchivo& ia, const DiccPartidosXFecha& ip, const DiccEquipoAPos& ep)
{
    Matriz<double> res(ia.cant_equipos, 1, 0);
    DiccEquipoVictorias v(victorias_equipos(ip));
    DiccEquipoDerrotas d(derrotas_equipos(ip));

    //it->first es el equipo, it->second es la posicion asignada al equipo
    for(DiccEquipoAPos::const_iterator it = ep.cbegin();it != ep.cend();++it)
        res[it->second][0] = double(v[it->first]) / double(v[it->first] + d[it->first]);

    return res;

}

//Varia el vector b de forma deterministica
void cambiar_matriz_b(const InfoArchivo& ia, DiccPartidosXFecha& ip, const DiccEquipoAPos& ep, Matriz<double>& b, unsigned int cant_b)
{
    // Cambio primeros cant_b resultados de los partidos
    
    while(cant_b > 0)
    {
        for(DiccPartidosXFecha::iterator it = ip.begin();it != ip.end();++it) // Recorro por las fechas jugadas
        {
            for(unsigned int i = 0; i < it->second.size();i++)//it->second es un vector con los partidos de la fecha
            {
                if(it->second[i].goles_i != it->second[i].goles_j)
                {
                    std::swap(it->second[i].goles_i, it->second[i].goles_j);
                    --cant_b;
                }

                if(cant_b == 0)
                    break;
            }
            if(cant_b == 0)
                    break;
        }
    }

    // Armo b con los resultados cambiados
    b = armar_vector_b(ia, ip, ep);
    // Deshago los cambios hechos antes
    while(cant_b > 0)
    {
        for(DiccPartidosXFecha::iterator it = ip.begin();it != ip.end();++it) // Recorro por las fechas jugadas
        {
            for(unsigned int i = 0; i < it->second.size();i++)//it->second es un vector con los partidos de la fecha
            {
                if(it->second[i].goles_i != it->second[i].goles_j)
                {
                    std::swap(it->second[i].goles_i, it->second[i].goles_j);
                    --cant_b;
                }

                if(cant_b == 0)
                    break;
            }
            if(cant_b == 0)
                    break;
        }
    }
}

int main(int argc, char* argv[]){

    //Filtramos los datos de entrada
	if (argc < 4){
		std::cout<<"Parametros de entrada insuficientes"<<'\n';
		return 1;
	}
	std::string input(argv[1]);
	std::string output(argv[2]);
	std::string mode(argv[3]);
   
    unsigned int cant_b = 0;

    if (argc >= 5)
    {
        std::string cb(argv[4]);
        cant_b = std::stoi(cb, nullptr);
    }

    bool mostrar_equipos = false;
    if (argc >= 6)
    {
        std::string mostrar(argv[5]);
        if(mostrar == "mostrar_equipos")
            mostrar_equipos = true;
    }

    Modo m;
    int modo = std::stoi(mode, nullptr);

    switch(modo)
    {
        case 0:
            m = CMM_EG;
            break;
        case 1:
            m = CMM_CL;
            break;
        case 2:
            m = WP;
            break;
        default:
            std::cout<<"ERROR: Valor de metodo a utilizar incorrecto\n";
            return 1;
    }

    //Vemos si existe el archivo de entrada, sino tiramos error
    std::ifstream test_in(input.c_str());
    if(!test_in.good())
    {
	    std::cout<<"ERROR: No se puede leer el archivo input\n";
	    return 1;
    }
    test_in.close();
    //Si existe el archivo output, lo borramos
    std::ifstream test_out(output.c_str());
    if(test_out.good())
    {
      test_out.close();
      std::remove(output.c_str()); 
    }
    //Cargo los datos del archivo input y los muestro en pantalla
    InfoArchivo info = leer_archivo(input);  
    //Cambio la cantidad de digitos con las que nos muestra los doubles en pantalla
    std::cout<<std::fixed;std::setprecision(20);   

    DiccPartidosXFecha ip = dividir_por_fecha(info);
    //Mapeo los equipos a indices que empiezan en 0 y son contiguos
    DiccEquipoAPos ep(posicion_por_equipo(ip));

    Matriz<double> b(armar_vector_b(info, ip, ep));

    Matriz<double> C(armar_matriz(info, ip, ep));

    if (cant_b == 0)
    {
        switch(m)
        {
            case CMM_EG:
                {
                    clock_t t = clock();
                    Matriz<double> ratings(resolver_gauss(C, b));

                    t = clock() - t;

                    std::cout<<((double)t)/((double)CLOCKS_PER_SEC)<<'\n';
                    escribir_output(ratings, output);
                    break;
                }
            case CMM_CL:
                {
                    clock_t t = clock();
                    Matriz<double> L(descomposicion_cholesky(C));
                    Matriz<double> ratings(resolver_chol(L, b));
                    t = clock() - t;

                    std::cout<<((double)t)/((double)CLOCKS_PER_SEC)<<'\n';
                    escribir_output(ratings, output);
                    break;
                }
            case WP:
                {
                    Matriz<double> ratings(calcular_wp(info, ip, ep));
                    escribir_output(ratings, output);
                    break;
                }
        }
    }
    else
    {
        switch(m)
        {
            case CMM_EG:
                {
                    clock_t total = 0;
                    for(unsigned int i = 1; i <= cant_b;i++)
                    {
                        clock_t t = clock();
                        Matriz<double> ratings(resolver_gauss(C, b));

                        t = clock() - t;
                        total += t;

                        cambiar_matriz_b(info, ip, ep, b, i);
                    }
                    std::cout<<((double)total)/((double)CLOCKS_PER_SEC)<<'\n';
                    break;
                }
            case CMM_CL:
                {
                    clock_t total = clock();
                    Matriz<double> L(descomposicion_cholesky(C));
                    total = clock() - total;
                    for(unsigned int i = 1;i <= cant_b;i++)
                    {
                        clock_t t = clock();
                        Matriz<double> ratings(resolver_chol(L, b));
                        t = clock() - t;
                        total += t;

                        cambiar_matriz_b(info, ip, ep, b, i);
                    }
                    std::cout<<((double)total)/((double)CLOCKS_PER_SEC)<<'\n';
                    break;
                }
            case WP:
                {
                    std::cout<<"No tiene sentido probar distintos b con WP\n";
                    break;
                }
        }
    }

    if(mostrar_equipos)
    {
        DiccEquipoAPos al_reves;
        for(DiccEquipoAPos::const_iterator it = ep.cbegin();it != ep.cend();++it)
            al_reves[it->second] = it->first;

        for(DiccEquipoAPos::const_iterator it = al_reves.cbegin();it != al_reves.cend();++it)
            std::cout<<it->second<<'\n';
    }

    return 0;
}
