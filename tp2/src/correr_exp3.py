import sys
import os

Kfold = 5
knn = 1
k_PCA = 30
modos = ["big", "red"]
PCAS = ["PCA", "PCA_pesos"]

comando = "./tp {0} {1} {2} {3} {4}"
path_in = "experimentos/experimento3/datos_entrada/{0}_kfold/{0}_K{1}_k{2}_fold{3}.in"
path_out = "derp.out"
path_medidas = "experimentos/experimento3/mediciones_{0}/{0}_{1}_K{2}_k{3}_fold{4}.out.medidas"

if os.system("python metnum.py build") != 0:
    print("Fallo al compilar")
    sys.exit(1)

for i in modos:
    for p in PCAS:
        for fold in range(1, Kfold+1):
            actual_in = path_in.format(i, Kfold, k_PCA, fold)
            actual_medidas = path_medidas.format(i, p, Kfold, k_PCA, fold)

            comando_final = comando.format(actual_in, path_out, p, str(knn), actual_medidas)

            print("Ejecutando " + comando_final)
            exit = os.system(comando_final)
            if exit != 0:
                print("ERROR CORRIENDO EL EXPERIMENTO 3 CON LOS SIGUIENTES PARAMETROS")
                print("Kfold: " + str(Kfold))
                print("Tipo imagen: " + i)
                print("Valor k de knn: " + str(k))
                print("Modo PCA: " + str(p))
                print("Valor k de PCA" + str(j))
                print("Fold actual: " + str(fold))
                sys.exit(1)
