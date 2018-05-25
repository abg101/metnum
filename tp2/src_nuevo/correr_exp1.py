import sys
import os

knn = [1, 3, 5, 7]
Kfolds = [1,2,3,5,7]
alphas_PCA = [1] + list(range(5, 45, 5))
tipo = ["big", "red"]
modo = ["kNN", "PCA"] #' 4 (ES_CARA)'
train_sizes = [20,50,75,100]


comando = "./tp2 -m {0} -i {1} -q {2} -o {3} -k {4} -a {5} -t {6} -r {7} -e"
comando_sin_pca = "./tp2 -m {0} -i {1} -q {2} -o {3} -k {4} -t {5} -r {6} -e"
#"train_{0}_size{3}_K{1}_fold{2}.csv".format(tipo, k_folds, str(i+1), str(train_size))

path_out_res_PCA = "experimentos/experimento_1/datos_de_salida_{0}/knn_{3}/pca_{5}/{0}_size{1}_K{2}_knn{3}_alpha{5}_fold{4}.csv"
path_medidas_PCA = "experimentos/experimento_1/mediciones_{0}/knn_{5}/pca_{5}/{0}_size{1}_K{2}_knn{3}_alpha{5}_fold{4}.csv.medidas"

path_in_train = "experimentos/experimento_1/datos_de_entrada_{0}/csv_train/train_{0}_size_{1}_K{2}_fold{3}.csv" 
path_in_test = "experimentos/experimento_1/datos_de_entrada_{0}/csv_test/test_{0}_size{1}_K{2}_fold{3}.csv"

path_out_res = "experimentos/experimento_1/datos_de_salida_{0}/knn_{3}/{0}_size{1}_K{2}_knn{3}_fold{4}.csv"
path_medidas = "experimentos/experimento_1/mediciones_{0}/knn_{3}/{0}_size{1}_K{2}_knn{3}_fold{4}.csv.medidas"

for i in tipo:
    for train_size in train_sizes:
        for kfold in Kfolds:
            for k in knn:
                for fold in range(1, kfold): #intera sobre los folds para hacer crossvalidation.
                    for m in modo:
                        if m == "PCA":
                            for alpha in alphas_PCA:
                                actual_in_train = path_in_train.format(i, train_size, kfold, fold )
                                actual_in_test = path_in_test.format(i,train_size ,kfold, fold)

                                actual_out_res = path_out_res_PCA.format(i, train_size, kfold, k, fold, alpha)
                                actual_medidas = path_medidas_PCA.format(i, train_size, kfold, k, fold, alpha)

                                comando_final = comando.format(modo.index(m),actual_in_train,actual_in_test, actual_out_res, str(k), str(alpha),i, actual_medidas)

                                print("Ejecutando " + comando_final)
                                exit = os.system(comando_final)
                                if exit != 0:
                                    print("ERROR CORRIENDO EL EXPERIMENTO 1 CON LOS SIGUIENTES PARAMETROS")
                                    print("Kfold: " + str(kfold))
                                    print("Tipo imagen: " + i)
                                    print("Valor k de knn: " + str(k))
                                    print("Modo: " + str(m))
                                    print("Valor alpha de PCA" + str(j))
                                    print("Fold actual: " + str(fold))
                                    sys.exit(1)
                        else:
                            actual_in_train =path_in_train.format(i, train_size, kfold, fold )
                            actual_in_test = path_in_test.format(i,train_size ,kfold, fold)

                            actual_out_res = path_out_res.format(i, train_size, kfold, k, fold)
                            actual_medidas = path_medidas.format(i, train_size, kfold, k, fold)

                            comando_final = comando_sin_pca.format(modo.index(m),actual_in_train,actual_in_test, actual_out_res, str(k), i, actual_medidas)

                            print("Ejecutando " + comando_final)
                            exit = os.system(comando_final)
                            if exit != 0:
                                print("ERROR CORRIENDO EL EXPERIMENTO 1 CON LOS SIGUIENTES PARAMETROS")
                                print("Kfold: " + str(kfold))
                                print("Tipo imagen: " + i)
                                print("Valor k de knn: " + str(k))
                                print("Modo: " + str(m))
                                print("Fold actual: " + str(fold))
                                sys.exit(1)