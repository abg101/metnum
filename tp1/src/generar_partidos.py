import sys
import random

if __name__=="__main__":
    if len(sys.argv) == 4:
        output = sys.argv[1]
        cant_equipos = int(sys.argv[2])
        cant_partidos = int(sys.argv[3])

        # Armo listo donde van las tuplas de los partidos, el primero de cada tupla es el que gano, el segundo el que
        # perdio
        partidos = list()

        # Armo lista con los ids de los equipos
        equipos = list(range(1, cant_equipos + 1))

        # Hago que cada equipo al menos juegue una vez
        for equipo_i in equipos:
            gano_o_perdio = random.randint(0, 1)
            equipo_j = equipo_i
            while equipo_j == equipo_i:
                equipo_j = random.choice(equipos)
            if gano_o_perdio == 0:
                partidos.append((equipo_i, equipo_j))
            else:
                partidos.append((equipo_j, equipo_i))

        # Hago el resto de los partidos de forma aleatoria
        for i in range(0, cant_partidos - cant_equipos):
            equipo_i = random.choice(equipos)
            equipo_j = equipo_i
            while equipo_i == equipo_j:
                equipo_j = random.choice(equipos)
            partidos.append((equipo_i, equipo_j))

        # Escribo los elementos de partidos en un archivo de nombre output
        # Asume que hay unsa sola fecha y que cada partido se gana con 1 a 0 en cuanto goles
        with open(output, mode='w+') as f:
            f.write("{0} {1}\n".format(cant_equipos, cant_partidos))
            for p in partidos:
                f.write("1 {0} 1 {1} 0\n".format(p[0], p[1]))
    else:
        print("Parametros de entrada insuficientes")
