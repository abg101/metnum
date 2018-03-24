import os

modos = ["CMM", "WP"]

path_resumen = "resumen_transitividad_{}.txt"

for m in modos:
    path = "transitividad_por_fecha/{0}/".format(m)
    files = os.listdir(path)
    files.sort()

    valores = []

    for fn in files:
        with open(path + fn, mode='r') as f:
            valores.append(f.readlines()[1])

    with open(path_resumen.format(m), mode='w') as f:
        for i in valores:
            f.write(i)
