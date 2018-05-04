import sys
import os

knn = 5
Kfold = 5
k_PCA = 1
modos = ["big", "red"]
PCAS = "PCA"

comando = "valgrind --tool=massif --pages-as-heap=yes --massif-out-file=temp_{2}_{4}.txt ./tp {0} {1} {2} {3};" + "grep mem_heap_B temp_{2}_{4}.txt > experimentos/experimento_1_memoria/memoria_sintruco_{5}_{2}_{4}.txt"
path_in = "experimentos/datos\ de\ entrada_{0}/{0}_K{1}_k{2}_fold{3}.in"
path_ignore = "derp.out"

if os.system("python metnum.py build") != 0:
    print("Fallo al compilar")
    sys.exit(1)

for i in modos:
    actual_in = path_in.format(i, Kfold, k_PCA, 1)
    comando_final = comando.format(actual_in, path_ignore, PCAS, knn, k_PCA, i)

    print("Ejecutando " + comando_final)
    exit = os.system(comando_final)
    if exit != 0:
        print("ERROR CORRIENDO EL EXPERIMENTO 1 CON LOS SIGUIENTES PARAMETROS")
        print("Kfold: " + str(Kfold))
        print("Tipo imagen: " + i)
        print("Valor k de knn: " + knn)
        print("Modo PCA: " + PCAS)
        print("Valor k de PCA" + k_PCA)
        print("Fold actual: " + 1)
        sys.exit(1)
