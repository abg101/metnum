import sys
from random import shuffle

# Como usar
# python3 armar_folds.py [k de kfold] [version base de datos] [k de pca] [path output]
# k de kfold debe estar entre 2 y 10
# version base de datos es el string "big" o "red"
# k de PCA, si es 0 entonces es sin PCA, si es mayor a 0 es con PCA
# path output es la carpeta donde se van a escribir los archivos


path_img_comun_corto = "data/ImagenesCaras/"
path_img_chicas_corto = "data/ImagenesCarasRed/"

path_img_comun = "data/ImagenesCaras/s{0}/{1}.pgm"
path_img_chicas = "data/ImagenesCarasRed/s{0}/{1}.pgm"

ancho_comun = 92
alto_comun = 112
ancho_chica = 23
alto_chica = 28

total_sujetos = 41


def armar_archivo_csv(path_archivo, path_datos, imgs):
    path_test = path_datos + ", {2},\n"
    with open(path_archivo, mode='w+') as f:
        for i in imgs.keys():
            for j in imgs[i]:
                if j % 10 == 0:
                    f.write(path_test.format(i, 10, i))
                else:
                    f.write(path_test.format(i, j % 10, i))


def armar_sujeto_x_imagen(sujetos_ignorados=None):
    res = dict()
    for i in range(1, total_sujetos + 1):
        if sujetos_ignorados is None or i not in sujetos_ignorados:
            for j in range((10 * (i-1)) + 1, (10 * i) + 1):
                res[j] = i
    return res


if __name__ == "__main__":
    k_folds = int(sys.argv[1])
    tipo = sys.argv[2].lower()

    if tipo == "big":
        ancho = ancho_comun
        alto = alto_comun
        path = path_img_comun
        path_corto = path_img_comun_corto
    elif tipo == "red":
        ancho = ancho_chica
        alto = alto_chica
        path = path_img_chicas
        path_corto = path_img_chicas_corto
    else:
        print("Parametro 2 incorrecto"+"\n")

    train_size = int(sys.argv[3])
    final_test_size = int(sys.argv[4]) #ultimo test. Normalmente se divide en un 10% final test.
    path_arc = sys.argv[5]

    # Armamos un dict que mapee una imagen con el sujeto que le correponde
    sujeto_x_imagen = armar_sujeto_x_imagen()
    print(sujeto_x_imagen)
    # Mezclamos los indices de las imagenes
    indices = list(sujeto_x_imagen.keys())
    indices.sort()
    print(sujeto_x_imagen)
    shuffle(indices)

    final_test_indice = int(len(indices) - (len(indices)*final_test_size/100))
    final_test = indices[final_test_indice:]
    indices = indices[:final_test_indice]

    limite = int(len(indices)*train_size/100)
    indices = indices[:limite] #elijo un porcentaje del dataset ya shuffleado.

    print(len(indices))
    print(indices)
    

    tam_x_fold = len(indices)/k_folds

    if k_folds == 1: #testeo con test set. no hay corss validation
            i = 0
            #path_out = path_arc + "{0}_K{1}_alpha{2}_fold{3}.in".format(tipo, k_folds, alpha, str(i+1))
            
            #path_out_train = path_arc +  "csv_train/"+ "train_{0}_size{1}.csv".format(tipo, str(train_size))
            #path_out_test = path_arc + "csv_test/"+"test_{0}_size{1}.csv".format(tipo,str(train_size))
            
            path_out_train = path_arc + "csv_train/" + "train_{0}_size{3}_K{1}_fold{2}.csv".format(tipo, k_folds, str(i+1), str(train_size))
            path_out_test = path_arc + "csv_test/" + "test_{0}_size{3}_K{1}_fold{2}.csv".format(tipo, k_folds, str(i+1), str(train_size))

            # Dividimos los indices de las imagenes en train y en test, para este fold
            datos_fold = dict()
            datos_fold["test"] = final_test
            datos_fold["train"] = indices

            # Armamos un dict que mapee un sujeto con las imagenes de train que le correponden
            train_imgs_x_sujeto = dict()
            for i in datos_fold["train"]:
                if sujeto_x_imagen[i] not in train_imgs_x_sujeto:
                    train_imgs_x_sujeto[sujeto_x_imagen[i]] = []
                train_imgs_x_sujeto[sujeto_x_imagen[i]].append(i)
            
            # Armamos un dict que mapee un sujeto con las imagenes de test que le correponden
            test_imgs_x_sujeto = dict()
            for i in datos_fold["test"]:
                if sujeto_x_imagen[i] not in test_imgs_x_sujeto:
                    test_imgs_x_sujeto[sujeto_x_imagen[i]] = []
                test_imgs_x_sujeto[sujeto_x_imagen[i]].append(i)
            
            armar_archivo_csv(path_out_train,path,train_imgs_x_sujeto)
            armar_archivo_csv(path_out_test,path,test_imgs_x_sujeto)
    else:   #K FOLD
        for i in range(0, k_folds):
            #path_out = path_arc + "{0}_K{1}_fold{2}.in".format(tipo, k_folds, str(i+1))
            
            path_out_train = path_arc + "csv_train/" + "train_{0}_size{3}_K{1}_fold{2}.csv".format(tipo, k_folds, str(i+1), str(train_size))
            path_out_test = path_arc + "csv_test/" + "test_{0}_size{3}_K{1}_fold{2}.csv".format(tipo, k_folds, str(i+1), str(train_size))
            
            # Dividimos los indices de las imagenes en train y en test, para este fold
            datos_fold = dict()
            datos_fold["test"] = [indices[j] for j in range(0, len(indices)) if i*tam_x_fold <= j < (i+1)*tam_x_fold]
            datos_fold["train"] = [j for j in indices if j not in datos_fold["test"]]

            # Armamos un dict que mapee un sujeto con las imagenes de train que le correponden
            train_imgs_x_sujeto = dict()
            for i in datos_fold["train"]:
                if sujeto_x_imagen[i] not in train_imgs_x_sujeto:
                    train_imgs_x_sujeto[sujeto_x_imagen[i]] = []
                train_imgs_x_sujeto[sujeto_x_imagen[i]].append(i)
            
            # Armamos un dict que mapee un sujeto con las imagenes de test que le correponden
            test_imgs_x_sujeto = dict()
            for i in datos_fold["test"]:
                if sujeto_x_imagen[i] not in test_imgs_x_sujeto:
                    test_imgs_x_sujeto[sujeto_x_imagen[i]] = []
                test_imgs_x_sujeto[sujeto_x_imagen[i]].append(i)
            
            armar_archivo_csv(path_out_train,path,train_imgs_x_sujeto)
            armar_archivo_csv(path_out_test,path,test_imgs_x_sujeto)        
