from graficos import grafico_linea, grafico_lineas, grafico_lineas_varios 
from leer_promedios import leer_promedios

carpeta_big = "experimentos/experimento 1/promedio_mediciones_big/knn_{0}/"
esquema_big = "promedio_big_PCA_K5_k{0}_fold5.txt"
carpeta_red = "experimentos/experimento 1/promedio_mediciones_red/knn_{0}/"
esquema_red = "promedio_red_PCA_K5_k{0}_fold5.txt"

esquema_big_pesos = "promedio_big_PCA_pesos_K5_k{0}_fold5.txt"
esquema_red_pesos = "promedio_red_PCA_pesos_K5_k{0}_fold5.txt"


rango = [1] + list(range(5, 105, 5))
knn_valores = [1, 3, 5, 7]
texto_eje_x = "Valor k de PCA"
knn_texto = ", usando kNN con k={0}"
mt = "{0} general según valor k de PCA"
y_labels = ["Hitrate","Precisión", "Recall"]
l = ["PCA + kNN canónico", "PCA + kNN distancia"]
m = ['r', 'b']
ylim = 100

for knn in knn_valores:

    # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

    # PCA + kNN canonico

    datos = leer_promedios(carpeta_big.format(knn), esquema_big, rango)
    eje_hitrate = []
    eje_precision = []
    eje_recall = []

    for i in sorted(datos.keys()):
        eje_hitrate.append(datos[i][1])
        eje_precision.append(datos[i][2])
        eje_recall.append(datos[i][3])

    # PCA + kNN variante distancia

    datos_pesos = leer_promedios(carpeta_big.format(knn), esquema_big_pesos, rango)
    eje_hitrate_pesos = []
    eje_precision_pesos = []
    eje_recall_pesos = []

    for i in sorted(datos_pesos.keys()):
        eje_hitrate_pesos.append(datos_pesos[i][1])
        eje_precision_pesos.append(datos_pesos[i][2])
        eje_recall_pesos.append(datos_pesos[i][3])

    # Agrupamos los datos y graficamos
    todos = list()
    
    d = [eje_hitrate, eje_hitrate_pesos]
    todos.append(d)

    d = [eje_precision, eje_precision_pesos]
    todos.append(d)

    d = [eje_recall, eje_recall_pesos]
    todos.append(d)

    for i in range(0, len(d) + 1):
        grafico_lineas(rango, todos[i], texto_eje_x, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knn_texto.format(knn), l, m, ylim, path="graf_big_{0}_knn{1}.png".format(y_labels[i],knn))

    # Leemos los promedios de las imagenes de tamaño reducido y luego las graficamos

    # PCA + kNN canonico
    datos = leer_promedios(carpeta_red.format(knn), esquema_red, rango)
    eje_hitrate = []
    eje_precision = []
    eje_recall = []

    for i in sorted(datos.keys()):
        eje_hitrate.append(datos[i][1])
        eje_precision.append(datos[i][2])
        eje_recall.append(datos[i][3])

    # PCA + kNN variante distancia

    datos_pesos = leer_promedios(carpeta_red.format(knn), esquema_red_pesos, rango)
    eje_hitrate_pesos = []
    eje_precision_pesos = []
    eje_recall_pesos = []

    for i in sorted(datos_pesos.keys()):
        eje_hitrate_pesos.append(datos_pesos[i][1])
        eje_precision_pesos.append(datos_pesos[i][2])
        eje_recall_pesos.append(datos_pesos[i][3])

    # Agrupamos los datos y graficamos
    todos = list()

    d = [eje_hitrate, eje_hitrate_pesos]
    todos.append(d)

    d = [eje_precision, eje_precision_pesos]
    todos.append(d)

    d = [eje_recall, eje_recall_pesos]
    todos.append(d)

    for i in range(0, len(d) + 1):
        grafico_lineas(rango, todos[i], texto_eje_x, "{} general(en %)".format(y_labels[i]), mt.format(y_labels[i]) + knn_texto.format(knn), l, m, ylim, path="graf_red_{0}_knn{1}.png".format(y_labels[i],knn))

