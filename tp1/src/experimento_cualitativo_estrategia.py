import os
import subprocess


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

    return cant_equipos, cant_partidos, partidosXfecha


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


def arg_max_rating(datos, e):
    datos_sin_e = {i:datos[i] for i in datos.keys() if i != e}
    return max(datos_sin_e, key=datos.get)


def cambiar_partidos(datos, e_i, e_max):
    fecha_final = max(datos.keys())
    partidos_fecha = datos[fecha_final]
    contrarios_a_cambiar = []

    for i in range(0, len(partidos_fecha)):
        if partidos_fecha[i][0] == e_i:
            if partidos_fecha[i][2] != e_max:
                contrarios_a_cambiar.append(partidos_fecha[i][2])
            datos[fecha_final][i] = (e_i, 0, e_max, 0) # fuerzo un empate
        elif partidos_fecha[i][2] == e_i:
            if partidos_fecha[i][0] != e_max:
                contrarios_a_cambiar.append(partidos_fecha[i][0])
            datos[fecha_final][i] = (e_i, 0, e_max, 0)  # fuerzo un empate

    for i in range(0, len(partidos_fecha)):
        if len(contrarios_a_cambiar) == 0:
            break

        if partidos_fecha[i][0] == e_max:
            datos[fecha_final][i] = (contrarios_a_cambiar.pop(), partidos_fecha[i][1],
                                    partidos_fecha[i][2], partidos_fecha[i][3])
        elif partidos_fecha[i][2] == e_max:
            datos[fecha_final][i] = (partidos_fecha[i][0], partidos_fecha[i][1],
                                    contrarios_a_cambiar.pop(), partidos_fecha[i][3])


def escribir_fixture(path, ce, cp, datos):
    with open(path, mode='w') as f:
        f.write("{0} {1}\n".format(ce, cp))
        ks = sorted(datos.keys())
        for k in ks:
            for p in datos[k]:
                f.write("{0} {1} {2} {3} {4}\n".format(k, p[0], p[1], p[2], p[3]))


def correr_experimento(path_in, path_out, path_equipos):
    programa = "./tp " + path_in + " " + path_out + " 1" + " 0" + " mostrar_equipos"
    output = subprocess.getoutput(programa)
    with open(path_equipos, mode="w") as f:
        f.write(output)


if __name__=="__main__":
    os.system("python metnum.py build")

    # Paths para archivos de entrada y salida
    parametro_datos_entrada = "experimentos/cualitativos/RANKINGS/datos/por_fecha/"
    parametro_datos_resultados = "experimentos/cualitativos/RANKINGS/resultados/por_fecha/CMM/"
    parametros_equipos_ordenados = "experimentos/cualitativos/RANKINGS/equipos_por_fecha/CMM/"

    parametro_datos_fixture = "experimentos/cualitativos/RANKINGS/experimento_estrategia/datos_fixture/"
    parametro_resultados_fixture = "experimentos/cualitativos/RANKINGS/experimento_estrategia/resultados_fixture/"
    parametro_equipos_fixture = "experimentos/cualitativos/RANKINGS/experimento_estrategia/equipos_fixture/"

    # Obtengo los archivos de entrada
    archivos_entrada = os.listdir(parametro_datos_entrada)
    archivos_entrada.sort()

    equipo_analizar = 1

    # Vemos la primer fecha aparte
    # Cambiamos el partido del equipo a analizar para que gane el partido
    fn = archivos_entrada[0]

    # Paths especificos de fn
    path_fixture = parametro_datos_fixture + fn.replace(".dat", ".dat_modificado")
    path_resultado_fixture = parametro_resultados_fixture + fn.replace(".dat", ".out")
    path_equipos_fixture = parametro_equipos_fixture + fn.replace(".dat", ".equipos")

    # Leo los datos de entrada
    ce, cp, pxf = leer_liga(parametro_datos_entrada + fn)

    # Para la primera fecha, nos aseguramos que equipo_analizar empate el partido, no importa contra quien
    primera_fecha = list(pxf.keys())[0]
    partidos = pxf[primera_fecha] 
    for i in range(0, len(partidos)):
        if partidos[i][0] == equipo_analizar:
            if partidos[i][1] <= partidos[i][3]:
                pxf[primera_fecha][i] = (partidos[i][0], 0, partidos[i][2], 0)
        elif partidos[i][2] == equipo_analizar:
            if partidos[i][3] <= partidos[i][1]:
                pxf[primera_fecha][i] = (partidos[i][0], 0, partidos[i][2], 0)

    escribir_fixture(path_fixture, ce, cp, pxf)
        
    correr_experimento(path_fixture, path_resultado_fixture, path_equipos_fixture)

    fixture = dict()
    fixture[primera_fecha] = pxf[primera_fecha]
    # Vemos el resto, cambiando los partidos del equipo a analizar para que juegue contra el de mayor rating, 
    # y que empate siempre

    for fn in archivos_entrada[1:]: 
        path_fixture = parametro_datos_fixture + fn.replace(".dat", ".dat_modificado")
        path_resultado_fixture = parametro_resultados_fixture + fn.replace(".dat", ".out")
        path_equipos_fixture = parametro_equipos_fixture + fn.replace(".dat", ".equipos")

        # Leemos los partidos de la liga original
        ce, cp, pxf = leer_liga(parametro_datos_entrada + fn)

        fecha_actual = sorted(list(pxf.keys()))[-1]  # Lista[-1] devuelve el ultimo elemento de la lista

        # Alteramos la liga original para que tenga los cambios anteriores, todavia no cambiamos la fecha actual
        fixture[fecha_actual] = pxf[fecha_actual]

        # Guardamos la liga alterada, para poder ver como cambiaron los ratings
        escribir_fixture(path_fixture, ce, cp, fixture)

        # Corremos el tp con la liga original alterada, para obtener los ratings
        correr_experimento(path_fixture, path_resultado_fixture, path_equipos_fixture)

        # Leemos los ratings y equipos de la liga alterada
        ratings = leer_ratings(path_resultado_fixture)
        equipos = leer_equipos(path_equipos_fixture)

        # Mapeamos los ratings por equipo
        rating_x_equipos = dict(zip(equipos, ratings))

        # Buscamos el equipo con mayor rating de la liga alterada
        equipo_max = arg_max_rating(rating_x_equipos, equipo_analizar)

        # Cambia los partidos de la ultima fecha para que equipo_analizar y e_max jueguen entre ellos
        cambiar_partidos(pxf, equipo_analizar, equipo_max)

        # Escribimos el fixture definitivo
        escribir_fixture(path_fixture, ce, cp, fixture)

        # Obtenemos los ratings del fixture definitivo
        correr_experimento(path_fixture, path_resultado_fixture, path_equipos_fixture)
