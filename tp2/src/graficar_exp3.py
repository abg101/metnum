from graficos import grafico_barras, grafico_barras_tiempo
from leer_promedios import leer_promedio

path = "experimentos/experimento3/promedios{0}_{1}/promedio_{1}_PCA_K5_k30_fold5.txt"

path_res2 = "experimentos/experimento 1/promedio_mediciones_{0}/knn_1/promedio_{0}_PCA_K5_k30_fold5.txt"

x_labels = ["Hitrate","Precisión", "Recall"]
y_label = "Porcentaje(%)"
m = ['r', 'b', 'g']

titulo = "Medidas de precisón según imágenes consideradas."

datos = dict()

for i in ["big", "red"]:

    datos["K-fold subconjunto"] = leer_promedio(path.format("",i))[1:]

    datos["Subconjunto contra conjunto"] = leer_promedio(path.format("_todos",i))[1:]

    datos["K-fold conjunto"] = leer_promedio(path_res2.format(i))[1:]

    grafico_barras(datos, x_labels, y_label, titulo, m)

