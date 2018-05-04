def leer_promedio(path):
    with open(path) as f:
        d = f.readlines()
        d = [i[:-1] for i in d]  # Le saco el newline a las lineas
        tiempo = float(d[1])
        hitrate = 100*float(d[3])
        precision = 100*float(d[5])
        recall = 100*float(d[7])
    return tiempo, hitrate, precision, recall


def leer_promedios(path_carpeta, esquema_archivo, rango):
    res = dict()
    for i in rango:
        path_archivo = path_carpeta + esquema_archivo.format(i)
        res[i] = leer_promedio(path_archivo)
    return res

