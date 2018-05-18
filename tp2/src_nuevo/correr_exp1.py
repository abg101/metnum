import sys
import os

knn = [1, 3, 5, 7]
Kfold = 5
k_PCA = [1] + list(range(5, 105, 5))
tipo = ["big", "red"]
modo = ["kNN", "PCA", "kNN_distancia", "PCA_distancia"] ''' 4 (ES_CARA)'''

comando = "./tp2 -m {0} -i {1} -q {2} -o {3} {4} {5} {6}"
path_in_train = "experimentos/datos\ de\ entrada_{0}/csv_train/{0}_K{1}_k{2}_fold{3}.csv"
path_in_test = "experimentos/datos\ de\ entrada_{0}/csv_test/{0}_K{1}_k{2}_fold{3}.csv"
path_out_res = "experimentos/experimento\ 1/datos\ de\ salida_{0}/{0}_K{1}_k{2}_fold{3}.csv"

path_medidas = "experimentos/experimento\ 1/mediciones_{0}/knn_{1}/{2}_{3}/{0}_K{4}_k{3}_fold{5}.csv.medidas"

if os.system("python metnum.py build") != 0: 'FALTA CAMBIAR ESTO'
    print("Fallo al compilar")
    sys.exit(1)

for i in tipo:
    for j in k_PCA:
        for k in knn:
            for m in modo:
                for fold in range(1, Kfold+1):
                    actual_in_train = path_in_train.format(i, Kfold, j, fold)
                    actual_in_test = path_in_test.format(i, Kfold, j, fold)
                    actual_out_res = path_out_res.format(i, Kfold, j, fold)
                    actual_medidas = path_medidas.format(i, k, m, j, Kfold, fold)

                    comando_final = comando.format(modo.index(m),actual_in_train,actual_in_test, actual_out_res, str(k), i, actual_medidas)

                    print("Ejecutando " + comando_final)
                    exit = os.system(comando_final)
                    if exit != 0:
                        print("ERROR CORRIENDO EL EXPERIMENTO 1 CON LOS SIGUIENTES PARAMETROS")
                        print("Kfold: " + str(Kfold))
                        print("Tipo imagen: " + i)
                        print("Valor k de knn: " + str(k))
                        print("Modo: " + str(m))
                        
                            print("Valor k de PCA" + str(j))
                        
                        print("Fold actual: " + str(fold))
                        sys.exit(1)
