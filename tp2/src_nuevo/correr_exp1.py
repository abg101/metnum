import sys
import os

knn = [1, 3, 5, 7]
Kfold = 5
k_PCA = [1] + list(range(5, 105, 5))
modos = ["big", "red"]
PCAS = ["PCA", "PCA_pesos"]

comando = "./tp {0} {1} {2} {3} {4}"
path_in = "experimentos/datos\ de\ entrada_{0}/{0}_K{1}_k{2}_fold{3}.in"
path_out = "experimentos/experimento\ 1/datos\ de\ salida_{0}/{0}_K{1}_k{2}_fold{3}.out"
path_medidas = "experimentos/experimento\ 1/mediciones_{0}/knn_{1}/{2}_{3}/{0}_K{4}_k{3}_fold{5}.out.medidas"

if os.system("python metnum.py build") != 0:
    print("Fallo al compilar")
    sys.exit(1)

for i in modos:
    for j in k_PCA:
        for k in knn:
            for p in PCAS:
                for fold in range(1, Kfold+1):
                    actual_in = path_in.format(i, Kfold, j, fold)
                    actual_out = path_out.format(i, Kfold, j, fold)
                    actual_medidas = path_medidas.format(i, k, p, j, Kfold, fold)

                    comando_final = comando.format(actual_in, actual_out, p, str(k), actual_medidas)

                    print("Ejecutando " + comando_final)
                    exit = os.system(comando_final)
                    if exit != 0:
                        print("ERROR CORRIENDO EL EXPERIMENTO 1 CON LOS SIGUIENTES PARAMETROS")
                        print("Kfold: " + str(Kfold))
                        print("Tipo imagen: " + i)
                        print("Valor k de knn: " + str(k))
                        print("Modo PCA: " + str(p))
                        print("Valor k de PCA" + str(j))
                        print("Fold actual: " + str(fold))
                        sys.exit(1)
