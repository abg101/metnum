import subprocess
import os
import sys
import math

def leer_liga(path):
    partidosXfecha = dict()
    with open(path, mode='r') as f:
        l = f.readline().split(" ")

        cant_equipos, cant_partidos = int(l[0]), int(l[1])

        for i in range(0, cant_partidos):
            l = f.readline().split(" ")
            fecha = int(l[0])
            equipo_i = int(l[1])
            goles_i = int(l[2])
            equipo_j = int(l[3])
            goles_j = int(l[4])
            if fecha not in partidosXfecha:
                partidosXfecha[fecha] = []

            partidosXfecha[fecha].append((equipo_i, goles_i, equipo_j, goles_j))

    return partidosXfecha


def leer_equipos(path, eliminar_primer_linea=True):
    equipos = []
    with open(path, mode='r') as f:
        d = f.readlines()
        if eliminar_primer_linea:
            d = d[1:]
        for i in d:
            equipos.append(int(i))
    return equipos


def leer_ratings(path):
    ratings = []
    with open(path, mode='r') as f:
        d = f.readlines()
        for i in d:
            ratings.append(float(i))
    return ratings


def ganador_partido(partido):
    return partido[0] if partido[1] > partido[3] else partido[2]


def perdedor_partido(partido):
    return partido[0] if partido[1] < partido[3] else partido[2]


def partidos_x_equipo(datos):
    pxe = dict()
    for f in datos.keys():
        for p in datos[f]:
            if ganador_partido(p) not in pxe:
                pxe[ganador_partido(p)] = {"gano": [], "perdio": []}
            if perdedor_partido(p) not in pxe:
                pxe[perdedor_partido(p)] = {"gano": [], "perdio": []}
            pxe[ganador_partido(p)]["gano"].append(perdedor_partido(p))
            pxe[perdedor_partido(p)]["perdio"].append(ganador_partido(p))
    return pxe


def transitividad_x_fecha(datos, ratings):
    cumple = []
    no_cumple = []
    pxe = partidos_x_equipo(datos)

    for ganador in pxe.keys():
        for perdedor in pxe[ganador]["gano"]:
            perdedores_del_perdedor = pxe[perdedor]["gano"]
            if ganador in perdedores_del_perdedor:
                perdedores_del_perdedor.remove(ganador)
            for per in perdedores_del_perdedor:
                if ratings[ganador] >= ratings[perdedor] >= ratings[per]:
                    cumple.append((ganador, perdedor, per))
                else:
                    no_cumple.append((ganador, perdedor, per))

    return cumple, no_cumple


def contar_posiciones(input_liga1,input_liga2,input_ratings1,input_ratings2, input_equipos1,input_equipos2, out_path, eliminar_primer_linea_equipos=True):
    pXf_1 = leer_liga(input_liga1)
    pXf_2 = leer_liga(input_liga2)
    equipos_resultado_1 = leer_equipos(input_equipos1, eliminar_primer_linea_equipos)
    equipos_resultado_2 = leer_equipos(input_equipos2, eliminar_primer_linea_equipos)
    ratings_resultado_1 = leer_ratings(input_ratings1)
    ratings_resultado_2 = leer_ratings(input_ratings2)

    if len(equipos_resultado_1) != len(ratings_resultado_1):
        print(len(equipos_resultado_1))
        print(len(ratings_resultado_1))
        print("WAT")
        return 0

    if len(equipos_resultado_2) != len(ratings_resultado_2):
        print(len(equipos_resultado_2))
        print(len(ratings_resultado_2))
        print("WAT")
        return 0

    eyr1 = [(equipos_resultado_1[i], ratings_resultado_1[i]) for i in range(0, len(equipos_resultado_1))]
    eyr1.sort(key=lambda tup: tup[1], reverse=True)
    eyr2 = [(equipos_resultado_2[i], ratings_resultado_2[i]) for i in range(0, len(equipos_resultado_2))]
    eyr2.sort(key=lambda tup: tup[1], reverse=True)

    dicc1 = {}
    dicc2 = {}
    j = 0
    for j in range(0,len(eyr1)):
        eq1 = eyr1[j][0]
        eq2 = eyr2[j][0]
        dicc1[eq1] = j + 1 
        dicc2[eq2] = j + 1

    difTotalPosiciones = 0    
    swapTotales = 0
    with open(out_path, mode='w') as f:
        f.write("Equipos, Diferencia de posicion y Swap\n")
        for i in eyr1:
            f.write("{0} {1} {2}\n".format(i[0], math.fabs(dicc1[i[0]] - dicc2[i[0]]) , 0 if (dicc1[i[0]] - dicc2[i[0]] == 0 ) else 1))
            difTotalPosiciones = difTotalPosiciones + math.fabs(dicc1[i[0]] - dicc2[i[0]])
            swapTotales = swapTotales + (0 if (dicc1[i[0]] - dicc2[i[0]] == 0 ) else 1)
        
        f.write("--------------------------\n")
        f.write("Total suma de diferencias, Total de swaps\n")
        f.write("{0} {1}\n".format(difTotalPosiciones, swapTotales))

if __name__ == "__main__":
    if len(sys.argv) == 5:
        input_liga1 = sys.argv[1]
        input_liga2 = sys.argv[2]
        input_ratings1 = sys.argv[3]
        input_ratings2 = sys.argv[4]
        input_equipos1 = sys.argv[5]
        input_equipos2 = sys.argv[6]
        out_path = sys.argv[7]

        contar_posiciones(input_liga, input_ratings, input_equipos, out_path)
    else:
        print("Parametros de entrada insuficientes")
