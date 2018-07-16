from graficos import grafico_linea, grafico_lineas, grafico_lineas_varios 
from leer_promedios import leer_promedios, leer_promedios_sinPCA, leer_promedios_conPCA

carpeta_big_sinPCA = "experimentos/experimento_1/promedio_mediciones_big/knn_{0}/"
carpeta_big_conPCA = "experimentos/experimento_1/promedio_mediciones_big/knn_{0}/pca_{1}/"
                
esquema_big_sinPCA = "promedio_big_knn_K{0}.txt"
esquema_big_conPCA = "promedio_big_pca_K{0}_alpha{1}.txt"

carpeta_red_sinPCA = "experimentos/experimento_1/promedio_mediciones_red/knn_{0}/"
carpeta_red_conPCA = "experimentos/experimento_1/promedio_mediciones_red/knn_{0}/pca_{1}/"
                
esquema_red_sinPCA = "promedio_red_knn_K{0}.txt"
esquema_red_conPCA = "promedio_red_pca_K{0}_alpha{1}.txt"

rango = [1] + list(range(5, 105, 5))
knn_valores = [1, 3, 5, 7]
Kfolds = [1,2,3,5,7]
alphas = [1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]
texto_eje_x_kNN = "Valor k de kNN"
texto_eje_x_PCA = "Valor alpha de PCA"
knn_texto = ", usando kNN con k={0}"
mk = "{0} general según valor k de kNN"
mt = "{0} general según valor alpha de PCA"
kf = ", usando kFold con K={0}"
knnkf = ", con k={0} de Knn y K={1} de kFold"
y_labels = ["Accuracy","Precisión", "Recall"]
l = ["kNN sin PCA", "kNN con PCA"]
m = ['r', 'b']
ylim = 100


#knn sin cross y sin pca, variando el k de knn: K=1
#knn con cross y sin pca, variando el k de knn:
for K in Kfolds:
    
    # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

    # kNN sin PCA

    datos_sinPCA_big = leer_promedios_sinPCA(carpeta_big_sinPCA, esquema_big_sinPCA,knn_valores, K)
    eje_accuracy_big = []
    eje_precision_big = []
    eje_recall_big = []

    for i in sorted(datos_sinPCA_big.keys()):
        eje_accuracy_big.append(datos_sinPCA_big[i][1])
        eje_precision_big.append(datos_sinPCA_big[i][2])
        eje_recall_big.append(datos_sinPCA_big[i][3])

    # Agrupamos los datos y graficamos
    todos_big = list()
    
    d = [eje_accuracy_big, eje_accuracy_sinPCA]
    todos_big.append(d)

    d = [eje_precision_big, eje_precision_sinPCA]
    todos_big.append(d)

    d = [eje_recall_big, eje_recall_sinPCA]
    todos_big.append(d)

    for i in range(0, len(d) + 1):
        grafico_lineas(rango, todos_big[i], texto_eje_x_kNN, "{} general(en %)".format(y_labels[i]), mk.format(y_labels[i]) + kf.format(K), l, m, ylim, path="graf_big_{0}_knn{1}_{2}.png".format(y_labels[i],knn,K))

    datos_sinPCA_red = leer_promedios_sinPCA(carpeta_big_sinPCA, esquema_big_sinPCA,knn_valores, K)
    eje_accuracy_red = []
    eje_precision_red = []
    eje_recall_red = []

    for i in sorted(datos_sinPCA_red.keys()):
        eje_accuracy_red.append(datos_sinPCA_red[i][1])
        eje_precision_red.append(datos_sinPCA_red[i][2])
        eje_recall_red.append(datos_sinPCA_red[i][3])

    # Agrupamos los datos y graficamos
    todos_red = list()
    
    d = [eje_accuracy_red, eje_accuracy_sinPCA]
    todos_red.append(d)

    d = [eje_precision_red, eje_precision_sinPCA]
    todos_red.append(d)

    d = [eje_recall_red, eje_recall_sinPCA]
    todos_red.append(d)

    for i in range(0, len(d) + 1):
        grafico_lineas(rango_red, todos[i], texto_eje_x_kNN, "{} general(en %)".format(y_labels[i]), mk.format(y_labels[i]) + kf.format(K), l, m, ylim, path="graf_red_{0}_knn{1}_{2}.png".format(y_labels[i],knn,K))

#knn sin cross y con pca, fijado el k de knn y variando los alpha:
#knn con cross y con pca, fijado el k de knn y variando los alpha:

for K in Kfolds:
    knn = 1
    
    # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

    # kNN sin PCA

    datos_conPCA = leer_promedios_conPCA(carpeta_big_conPCA, esquema_big_conPCA,alphas,knn, K)
    eje_accuracy = []
    eje_precision = []
    eje_recall = []

    for i in sorted(datos_conPCA.keys()):
        eje_accuracy.append(datos_conPCA[i][1])
        eje_precision.append(datos_conPCA[i][2])
        eje_recall.append(datos_conPCA[i][3])

    # Agrupamos los datos y graficamos
    todos = list()
    
    d = [eje_accuracy, eje_accuracy_conPCA]
    todos.append(d)

    d = [eje_precision, eje_precision_conPCA]
    todos.append(d)

    d = [eje_recall, eje_recall_conPCA]
    todos.append(d)

    for i in range(0, len(d) + 1):
        grafico_lineas(rango, todos[i], texto_eje_x_kNN, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knnkf.format(knn,K), l, m, ylim, path="graf_big_{0}_pca{1}_knn{2}_{3}.png".format(y_labels[i],alpha,knn,K))




    for i in range(0, len(d) + 1):
        grafico_lineas(rango, todos[i], texto_eje_x_kNN, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knnkf.format(knn,K), l, m, ylim, path="graf_red_{0}_pca{1}_knn{2}_{3}.png".format(y_labels[i],alpha,knn,K))















"""
for knn in knn_valores:
    for a in alphas:
        for K in Kfolds:
            for j in range(1,K + 1):

                # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

                # kNN sin PCA

                datos_sinPCA = leer_promedios_sinPCA(carpeta_big_sinPCA.format(knn), esquema_big_sinPCA, rango, K, 100, j)
                eje_accuracy = []
                eje_precision = []
                eje_recall = []

                for i in sorted(datos_sinPCA.keys()):
                    eje_accuracy.append(datos_sinPCA[i][1])
                    eje_precision.append(datos_sinPCA[i][2])
                    eje_recall.append(datos_sinPCA[i][3])

                # kNN con PCA

                datos_conPCA = leer_promedios_conPCA(carpeta_big_conPCA.format(knn,a), esquema_big_conPCA, rango, K, 100, j, a)
                eje_accuracy_conPCA = []
                eje_precision_conPCA = []
                eje_recall_conPCA = []

                for i in sorted(datos_conPCA.keys()):
                    eje_accuracy_conPCA.append(datos_conPCA[i][1])
                    eje_precision_conPCA.append(datos_conPCA[i][2])
                    eje_recall_conPCA.append(datos_conPCA[i][3])

                # Agrupamos los datos y graficamos
                todos = list()
                
                d = [eje_accuracy, eje_accuracy_conPCA]
                todos.append(d)

                d = [eje_precision, eje_precision_conPCA]
                todos.append(d)

                d = [eje_recall, eje_recall_conPCA]
                todos.append(d)

                for i in range(0, len(d) + 1):
                    grafico_lineas(rango, todos[i], texto_eje_x, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knn_texto.format(knn), l, m, ylim, path="graf_big_{0}_knn{1}.png".format(y_labels[i],knn))

                # Leemos los promedios de las imagenes de tamaño reducido y luego las graficamos

                # PCA + kNN canonico
                datos_sinPCA = leer_promedios_sinPCA(carpeta_red_sinPCA.format(knn), esquema_red_sinPCA, rango, K, 100, j)
                eje_accuracy = []
                eje_precision = []
                eje_recall = []

                for i in sorted(datos_sinPCA.keys()):
                    eje_accuracy.append(datos_sinPCA[i][1])
                    eje_precision.append(datos_sinPCA[i][2])
                    eje_recall.append(datos_sinPCA[i][3])

                # PCA + kNN variante distancia

                datos_conPCA = leer_promedios_conPCA(carpeta_red_conPCA.format(knn,a), esquema_red_conPCA, rango, K, 100, j, a)
                eje_accuracy_conPCA = []
                eje_precision_conPCA = []
                eje_recall_conPCA = []

                for i in sorted(datos_conPCA.keys()):
                    eje_accuracy_conPCA.append(datos_conPCA[i][1])
                    eje_precision_conPCA.append(datos_conPCA[i][2])
                    eje_recall_conPCA.append(datos_conPCA[i][3])

                # Agrupamos los datos y graficamos
                todos = list()

                d = [eje_accuracy, eje_accuracy_conPCA]
                todos.append(d)

                d = [eje_precision, eje_precision_conPCA]
                todos.append(d)

                d = [eje_recall, eje_recall_conPCA]
                todos.append(d)

                for i in range(0, len(d) + 1):
                    grafico_lineas(rango, todos[i], texto_eje_x, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knn_texto.format(knn), l, m, ylim, path="graf_red_{0}_knn{1}.png".format(y_labels[i],knn))
"""
