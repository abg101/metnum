#ifndef NORMAS_AUX_HPP
#define NORMAS_AUX_HPP

#include "matriz.hpp"
#include <cassert>
#include <cmath>

double norma_2_vec(const Matriz<double>& v)
{
    assert((v.columnas() == 1 && v.filas() >= 1) ||
           (v.columnas() >= 1 && v.filas() == 1));

    double suma = 0.0;
    if(v.columnas() == 1)
    {
        for(int i = 0;i < v.filas();i++)
           suma += v[i][0] * v[i][0];

        suma = std::sqrt(suma);
    }
    else
    {
        for(int i = 0;i < v.columnas();i++)
           suma += v[0][i] * v[0][i];

        suma = std::sqrt(suma);
    }
    return suma;
}

double norma_1(const Matriz<double>& v)
{
    assert((v.columnas() == 1 && v.filas() >= 1) ||
           (v.columnas() >= 1 && v.filas() == 1));

    double suma = 0.0;
    if(v.columnas() == 1)
        for(int i = 0;i < v.filas();i++)
           suma += std::abs(v[i][0]);
    else
        for(int i = 0;i < v.columnas();i++)
           suma += std::abs(v[0][i]);

    return suma;

}

double norma_infinito(const Matriz<double>& v)
{
    assert((v.columnas() == 1 && v.filas() >= 1) ||
           (v.columnas() >= 1 && v.filas() == 1));

    double max = 0.0;
    if(v.columnas() == 1)
    {
        for(int i = 0;i < v.filas();i++)
           if(std::abs(v[i][0]) > max)
               max = std::abs(v[i][0]);
    }
    else
    {
        for(int i = 0;i < v.columnas();i++)
           if(std::abs(v[0][i]) > max)
               max = std::abs(v[0][i]);
    }

    return max;

}

#endif
