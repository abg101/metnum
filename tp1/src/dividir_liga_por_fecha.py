#!/usr/bin/env python3
import sys

# CORRER CON PYTHON3
#MODO DE USO: python3 dividir_liga_por_fecha archivo_entrada prefijo_salida(ej:nba) cant_intervalo


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


def cant_equipos_hasta_fecha(datos, fechas, fecha_inicial, fecha_final):
    equipos = []
    for i in range(fecha_inicial, fecha_final):
        for d in datos[fechas[i]]:
            if d[0] not in equipos:
                equipos.append(d[0])
            if d[2] not in equipos:
                equipos.append(d[2])
    return len(equipos)


def cant_partidos_hasta_fecha(datos, fechas, fecha_inicial, fecha_final):
    cant_p = 0
    for i in range(fecha_inicial, fecha_final):
        cant_p += len(datos[fechas[i]])
    return cant_p


def partidos_hasta_fecha(datos, fechas, fecha_inicial, fecha_final):
    partidos = dict()
    for i in range(fecha_inicial, fecha_final):
        partidos[fechas[i]] = datos[fechas[i]]
    return partidos


if __name__ == "__main__":
    if len(sys.argv) == 4:
        input = sys.argv[1]
        out_path = sys.argv[2]
        intervalo = int(sys.argv[3])


        pXf = leer_liga(input)

        fechas = list(pXf.keys())
        fechas.sort()

        fechas_indices_intervalos = [i for i in range(0, len(fechas) + 1, intervalo)]

        for f in fechas_indices_intervalos[1:]:
            ce = cant_equipos_hasta_fecha(pXf, fechas, 0, f)
            cp = cant_partidos_hasta_fecha(pXf, fechas, 0, f)

            partidos_intervalo = partidos_hasta_fecha(pXf, fechas, 0, f)

            output = out_path + "_" + str(fechas[0]) + "_" + str(fechas[f-1]) + ".dat"

            with open(output, mode='w') as out:
                out.write("{0} {1}\n".format(ce, cp))
                for i in range(0, f):
                    for p in partidos_intervalo[fechas[i]]:
                        out.write("{0} {1} {2} {3} {4}\n".format(str(fechas[i]), p[0], p[1], p[2], p[3]))

    else:
        print("Parametros de entrada insuficientes")
