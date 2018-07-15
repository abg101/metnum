from graficos import grafico_linea, grafico_lineas, grafico_lineas_varios 
from leer_promedios import leer_promedios

carpeta_big_sinPCA = "experimentos/experimento_1/promedio_mediciones_big/knn_{0}/"
carpeta_big_conPCA = "experimentos/experimento_1/promedio_mediciones_big/knn_{0}/pca_{1}/"
                
esquema_big_sinPCA = "promedio_big_knn_K{0}_fold{1}.txt"
esquema_big_conPCA = "promedio_big_pca_K{0}_alpha{1}_fold{2}.txt"

carpeta_red_sinPCA = "experimentos/experimento_1/promedio_mediciones_red/knn_{0}/"
carpeta_red_conPCA = "experimentos/experimento_1/promedio_mediciones_red/knn_{0}/pca_{1}/"
                
esquema_red_sinPCA = "promedio_red_knn_K{0}_fold{1}.txt"
esquema_red_conPCA = "promedio_red_pca_K{0}_alpha{1}_fold{2}.txt"

rango = [1] + list(range(5, 105, 5))
knn_valores = [1, 3, 5, 7]
Kfolds = [1,2,3,5,7]
alphas = [1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]
texto_eje_x = "Valor k de PCA"
knn_texto = ", usando kNN con k={0}"
mt = "{0} general según valor alpha de PCA"
y_labels = ["Accuracy","Precisión", "Recall"]
l = ["kNN sin PCA", "kNN con PCA"]
m = ['r', 'b']
ylim = 100



for K in Kfolds:
    for a in alphas:
        for j in range(1,K + 1):
            for knn in knn_valores:
            
                # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

                # kNN sin PCA

                datos_sinPCA = leer_promedios_sinPCA(carpeta_big_sinPCA.format(knn), esquema_big_sinPCA, K, j)
                eje_accuracy = []
                eje_precision = []
                eje_recall = []

                for i in sorted(datos_sinPCA.keys()):
                    eje_accuracy.append(datos_sinPCA[i][1])
                    eje_precision.append(datos_sinPCA[i][2])
                    eje_recall.append(datos_sinPCA[i][3])

                # kNN con PCA

                datos_conPCA = leer_promedios_conPCA(carpeta_big_conPCA.format(knn,a), esquema_big_conPCA, K, a, j)
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

                for i in range(0, len(d) + 1):
                    grafico_lineas(rango, todos[i], texto_eje_x, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knn_texto.format(knn), l, m, ylim, path="graf_red_{0}_knn{1}.png".format(y_labels[i],knn))


















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
