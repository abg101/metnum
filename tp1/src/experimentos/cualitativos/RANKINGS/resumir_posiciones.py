import os

modos = ["CMM", "WP"]

path_resumen_suma = "resumen_posiciones_suma_{}.txt"
path_resumen_swaps = "resumen_posiciones_swaps_{}.txt"

for m in modos:
    path = "posiciones_por_fecha/{0}/".format(m)
    files = os.listdir(path)
    files.sort()

    sumas = []
    swaps = []

    for fn in files:
        with open(path + fn, mode='r') as f:
            d = f.readlines()
            datos = d[-1]
            datos = datos.split(" ")

            sumas.append(float(datos[0]))
            swaps.append(int(datos[1]))

    with open(path_resumen_suma.format(m), mode='w') as f:
        for i in sumas:
            f.write("{}\n".format(i))

    with open(path_resumen_swaps.format(m), mode='w') as f:
        for i in swaps:
            f.write("{}\n".format(i))

