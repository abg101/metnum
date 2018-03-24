#!/usr/bin/env python3
import sys


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


def contar_transitividad(input_liga, input_ratings, input_equipos, out_path, eliminar_primer_linea_equipos=True):
    pXf = leer_liga(input_liga)
    equipos_resultado = leer_equipos(input_equipos, eliminar_primer_linea_equipos)
    ratings_resultado = leer_ratings(input_ratings)

    if len(equipos_resultado) != len(ratings_resultado):
        print(len(equipos_resultado))
        print(len(ratings_resultado))
        print("WAT")
        return 0

    ratingsXequipos = dict(zip(equipos_resultado, ratings_resultado))

    c, nc = transitividad_x_fecha(pXf, ratingsXequipos)

    eyr = [(equipos_resultado[i], ratings_resultado[i]) for i in range(0, len(equipos_resultado))]
    eyr.sort(key=lambda tup: tup[1], reverse=True)

    with open(out_path, mode='w') as f:
        f.write("Porcentaje de casos en donde se cumple la relación\n")
        f.write("{0}%\n".format((len(c)/(len(c)+len(nc)))*100 if (len(c) + len(nc)) > 0 else 0))
        f.write("Casos en los que se cumple la relacion\n{0}\nCasos en los que no se cumple\n{1}\n".format(len(c), len(nc)))

        f.write("--------------------------\n")
        f.write("Equipos ordenados por rating, de mayor a menor\n")
        for i in eyr:
            f.write("{0} {1}\n".format(i[0], i[1]))

        f.write("--------------------------\n")
        f.write("Cumplen transitividad(ordenado A B C | rating_A rating_B rating_C, tal que A >= B >= C)\n")
        for i in c:
            f.write("{0} {1} {2} | {3} {4} {5}\n".format(i[0], i[1], i[2], ratingsXequipos[i[0]], ratingsXequipos[i[1]],
                                                         ratingsXequipos[i[2]]))

        f.write("--------------------------\n")
        f.write("No cumplen transitividad\n")
        f.write("--------------------------\n")
        for i in nc:
            f.write("{0} {1} {2} | {3} {4} {5}\n".format(i[0], i[1], i[2], ratingsXequipos[i[0]], ratingsXequipos[i[1]],
                                                         ratingsXequipos[i[2]]))


if __name__ == "__main__":
    if len(sys.argv) == 5:
        input_liga = sys.argv[1]
        input_ratings = sys.argv[2]
        input_equipos = sys.argv[3]
        out_path = sys.argv[4]

        contar_transitividad(input_liga, input_ratings, input_equipos, out_path)
    else:
        print("Parametros de entrada insuficientes")
