import sys
from random import shuffle

# Como usar
# python3 armar_folds.py [k de kfold] [version base de datos] [k de pca] [path output]
# k de kfold debe estar entre 2 y 10
# version base de datos es el string "big" o "red"
# k de PCA, debe ser mayor a 0
# path output es la carpeta donde se van a escribir los archivos

# Arma los folds en base a un subconjunto de las imagenes, en el cual solo estan las primeras 5 imagenes de cada sujeto

path_img_comun_corto = "../data/ImagenesCaras/"
path_img_chicas_corto = "../data/ImagenesCarasRed/"

path_img_comun = "../data/ImagenesCaras/s{0}/{1}.pgm"
path_img_chicas = "../data/ImagenesCarasRed/s{0}/{1}.pgm"

ancho_comun = 92
alto_comun = 112
ancho_chica = 23
alto_chica = 28

total_sujetos = 41

def armar_archivo(path_archivo, path_datos_corto, path_datos, ancho, alto, cant_sujetos, max_imgs_train, k, imgs_train, cant_imgs_test, imgs_test):
    path_test = path_datos + " {2}\n"
    with open(path_archivo, mode='w') as f:
        f.write(path_datos_corto)
        f.write(" {} ".format(alto))
        f.write("{} ".format(ancho))
        f.write("{} ".format(cant_sujetos))
        f.write("{} ".format(max_imgs_train))
        f.write("{}\n".format(k))

        for i in imgs_train.keys():
            f.write("s{}/".format(i))
            for j in sorted(imgs_train[i]):
                if j % 10 == 0:
                    f.write(" {}".format(10))
                else:
                    f.write(" {}".format(j % 10))
            f.write("\n")

        f.write("{}\n".format(cant_imgs_test))
        for i in imgs_test.keys():
            for j in imgs_test[i]:
                if j % 10 == 0:
                    f.write(path_test.format(i, 10, i))
                else:
                    f.write(path_test.format(i, j % 10, i))


def armar_sujeto_x_imagen(sujetos_ignorados=None):
    res = dict()
    for i in range(1, total_sujetos + 1):
        if sujetos_ignorados is None or i not in sujetos_ignorados:
            for j in range((10 * (i-1)) + 1, (10 * i) - 4):
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
        print("Parametro 2 incorrecto")

    k = int(sys.argv[3])# Valor k para PCA
    path_arc = sys.argv[4]

    # Armamos un dict que mapee una imagen con el sujeto que le correponde
    sujeto_x_imagen = armar_sujeto_x_imagen()

    # Mezclamos los indices de las imagenes
    indices = list(sujeto_x_imagen.keys())
    indices.sort()
    shuffle(indices)

    tam_x_fold = len(indices)/k_folds
    for i in range(0, k_folds):
        path_out = path_arc + "{0}_K{1}_k{2}_fold{3}.in".format(tipo, k_folds, k, str(i+1))
        # Dividimos los indices de las imagenes en train y en test, para este fold
        datos_fold = dict()
        datos_fold["test"] = [indices[j] for j in range(0, len(indices)) if i*tam_x_fold <= j < (i+1)*tam_x_fold]
        datos_fold["train"] = [j for j in indices if j not in datos_fold["test"]]

        # Contamos la cantidad maxima de imagenes por sujeto
        contar_imgs_x_sujeto = [0] * (total_sujetos+1)
        for j in datos_fold["train"]:
            contar_imgs_x_sujeto[sujeto_x_imagen[j]] += 1
        max_imgs_x_sujeto = max(contar_imgs_x_sujeto)

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

        armar_archivo(path_out, path_corto, path, ancho, alto, len(train_imgs_x_sujeto.keys()),
                      max_imgs_x_sujeto, k, train_imgs_x_sujeto, len(datos_fold["test"]), test_imgs_x_sujeto)


