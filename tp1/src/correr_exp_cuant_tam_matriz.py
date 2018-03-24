import subprocess
import os

if __name__ == "__main__":
    os.system("python metnum.py build")

    parametro_entrada = "experimentos/cuantitativos/tamaño\ matriz/datos/equipos_{0}.in "
    parametro_salida = "experimentos/cuantitativos/tamaño\ matriz/resultados/{0}/equipos_{1}.out "
    tiempos = "experimentos/cuantitativos/tamaño matriz/tiempos/{0}/equipos_{1}.tiempo"
    modo = [0, 1]
    modo_a_string = {0: "EG", 1: "CL"}

    cant_equipos = list(range(100, 2000, 100))
    #cant_equipos = [5, 10, 15, 20, 30, 40, 50]
    cant_equipos2 = [5, 10, 15, 20, 30, 40, 50]
    cant_equipos = cant_equipos ++ cant_equipos2
    CANT_MEDICIONES = 10

    for e in cant_equipos:
        for m in modo:
            mediciones = []
            for i in range(0, CANT_MEDICIONES):
                programa = "./tp " + parametro_entrada.format(e) +\
                            parametro_salida.format(modo_a_string[m], e) +\
                            str(m)
                print("Corriendo " + programa)
                out = subprocess.getoutput(programa)
                print(out)
                t = float(out)
                mediciones.append(t)
            with open(tiempos.format(modo_a_string[m], e), mode='w') as f:
                f.write("{0:f}\n".format(sum(mediciones)/len(mediciones)))
