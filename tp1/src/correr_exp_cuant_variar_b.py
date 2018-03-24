import subprocess
import os

if __name__ == "__main__":
    os.system("python metnum.py build")

    parametro_entradas = ["../data/nba_2016_scores.dat", "../data/atp_matches_2015.dat"]
    parametro_salida = " no_importa.out "
    tiempos = "experimentos/cuantitativos/variar b/tiempos/{0}/cant_b_{1}_{2}.tiempo"
    modo = [0, 1]
    modo_a_string = {0: "EG", 1: "CL"}

    cant_b = list(range(1, 11, 1))
    cant_b.append(50)
    cant_b.append(100)
    CANT_MEDICIONES = 10

    for b in cant_b:
        for m in modo:
            for p in parametro_entradas:
                mediciones = []
                for i in range(0, CANT_MEDICIONES):
                    programa = "./tp " + p +\
                                parametro_salida +\
                                str(m) + " " +\
                                str(b)
                    print("Corriendo " + programa)
                    out = subprocess.getoutput(programa)
                    print(out)
                    t = float(out)
                    mediciones.append(t)
                with open(tiempos.format(modo_a_string[m], p[8:], str(b)), mode='w') as f:
                    f.write("{0:f}\n".format(sum(mediciones)/len(mediciones)))
