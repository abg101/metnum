from graficos import grafico_linea, grafico_lineas, grafico_lineas
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
knn_texto = ", usando kNN {0} con k={1}"
mt = "{0} general según valor k de PCA"
l = ["PCA + kNN canónico", "PCA + kNN distancia"]
m = ['r', 'b']

for knn in knn_valores:

    # Leemos los promedios de las imagenes de tamaño original y luego las graficamos

    # PCA + kNN canonico

    datos = leer_promedios(carpeta_big.format(knn), esquema_big, rango)
    eje_tiempo = []

    for i in sorted(datos.keys()):
        eje_tiempo.append(datos[i][0])

    # PCA + kNN variante distancia

    datos_pesos = leer_promedios(carpeta_big.format(knn), esquema_big_pesos, rango)
    eje_tiempo_pesos = []

    for i in sorted(datos_pesos.keys()):
        eje_tiempo_pesos.append(datos_pesos[i][0])

    # Agrupamos los datos y graficamos
    d = list()
    d.append(eje_tiempo)
    d.append(eje_tiempo_pesos)
    ylims = max(max(d[0]), max(d[1]))
    grafico_lineas(rango, d, texto_eje_x, "Tiempo de computo(en ticks)", "Tiempo de cómputo según k de PCA para {}-nn".format(knn), l, m, max_y_lim=ylims, path="graf_tiempo_big_knn{}.png".format(knn))

    # Leemos los promedios de las imagenes de tamaño reducido y luego las graficamos

    # PCA + kNN canonico
    datos = leer_promedios(carpeta_red.format(knn), esquema_red, rango)
    eje_tiempo = []

    for i in sorted(datos.keys()):
        eje_tiempo.append(datos[i][0])

    # PCA + kNN variante distancia

    datos_pesos = leer_promedios(carpeta_red.format(knn), esquema_red_pesos, rango)
    eje_tiempo_pesos = []

    for i in sorted(datos_pesos.keys()):
        eje_tiempo_pesos.append(datos_pesos[i][0])

    # Agrupamos los datos y graficamos
    d = list()
    d.append(eje_tiempo)
    d.append(eje_tiempo_pesos)
    ylims = max(max(d[0]), max(d[1]))

    grafico_lineas(rango, d, texto_eje_x, "Tiempo de computo(en ticks)", "Tiempo de cómputo según k de PCA para {}-nn".format(knn), l, m, max_y_lim=ylims, path="graf_tiempo_red_knn{}.png".format(knn))
