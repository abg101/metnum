import os
import matplotlib.pyplot as plt
import numpy as np


def dibujar(x, y1, y2, labelx, labely, labeldata1, labeldata2, path):
    y3 = [y1[i] - y2[i] for i in range(0, len(y1))]

    plt.figure(figsize=(8, 6))
    plt.xlabel(labelx, fontsize=18)
    plt.ylabel(labely, fontsize=18)
    plt.ylim(-(max(y3) + 5), max(y3) + 5)
    plt.plot(x, y3, 'b', label=labeldata1)
    plt.plot(x, [0]*len(x), 'r')
    #plt.plot(x, y2, 'r', label=labeldata2)
    #plt.legend()
    plt.savefig(path)


def leer_resumen(path, type):
    res = {"CMM": [], "WP": []}
    for i in ["CMM", "WP"]:
        with open(path.format(i), mode='r') as f:
            for line in f.readlines():
                res[i].append(type(line))
    return res

path_resumen_suma = "experimentos/cualitativos/RANKINGS/resumen_posiciones_suma_{}.txt"
path_resumen_swaps = "experimentos/cualitativos/RANKINGS/resumen_posiciones_swaps_{}.txt"

sumas = leer_resumen(path_resumen_suma, float)
swaps = leer_resumen(path_resumen_swaps, int)

x = list(range(1, len(sumas["CMM"]) + 1))

dibujar(x, sumas["CMM"], sumas["WP"], "Cantidad de fechas jugadas", "Diferencia entre posiciones",
        "CMM", "WP", "graf_exp_pos_sumas.png")

dibujar(x, swaps["CMM"], swaps["WP"], "Cantidad de fechas jugadas", "Diferencia en la cantidad de swaps",
        "CMM", "WP", "graf_exp_pos_swaps.png")


