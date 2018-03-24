import os
import matplotlib.pyplot as plt
import numpy as np
from experimento_cualitativo_estrategia import leer_liga, leer_equipos, leer_ratings


def dibujar2(x, y1, y2, labelx, labely, labeldata1, labeldata2):
    plt.figure(figsize=(8, 6))
    plt.xlabel(labelx, fontsize=18)
    #plt.xticks(np.arange(0, max(x)+1, 1))
    plt.ylim(0, 1)
    plt.ylabel(labely, fontsize=18)
    plt.plot(x, y1, 'b', label=labeldata1)
    plt.plot(x, y2, 'r', label=labeldata2)
    plt.legend()
    plt.show()
    plt.savefig("herp.png")


def dibujar(x, y1, labelx, labely):
    plt.figure(figsize=(8, 6))
    plt.xlabel(labelx, fontsize=18)
    plt.xlim(0, max(x)+1)
    plt.ylim(0, 31)
    plt.ylabel(labely, fontsize=18)
    plt.plot(x, y1, 'r')
    plt.savefig("herp.png")


path_archivos = "experimentos/cualitativos/RANKINGS/datos/por_fecha/"

path_resultados_alterados = "experimentos/cualitativos/RANKINGS/experimento_estrategia/resultados_fixture/"
path_equipos_alterados = "experimentos/cualitativos/RANKINGS/experimento_estrategia/equipos_fixture/"

archivos = sorted(os.listdir(path_archivos))

equipo_analizado = 1

datos_alterados = []

for fn in archivos:
    resultados_alterados = path_resultados_alterados + fn.replace(".dat", ".out")
    equipos_alterados = path_equipos_alterados + fn.replace(".dat", ".equipos")

    ratings = leer_ratings(resultados_alterados)
    equipos = leer_equipos(equipos_alterados)

    equipos_y_ratings = [(equipos[i], ratings[i]) for i in range(0, len(equipos))]
    equipos_y_ratings.sort(key=lambda x:x[1])

    posicion = [i + 1 for i in range(0, len(equipos_y_ratings)) if equipos_y_ratings[i][0] == equipo_analizado]
    datos_alterados.append(posicion[0])

    #ratings_x_equipos = dict(zip(equipos, ratings))
    #datos_alterados.append(ratings_x_equipos[equipo_analizado])

x = list(range(1, len(datos_alterados) + 1))

dibujar(x, datos_alterados, "Fechas jugadas", "Posicion")




