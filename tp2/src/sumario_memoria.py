from graficos import grafico_boxplot
import pandas as pd
import numpy as np

path_in = "experimentos/experimento_1_memoria/memoria_{0}_{1}_{2}.txt"
path_in_sin_truco = "experimentos/experimento_1_memoria/memoria_sintruco_{0}_{1}_{2}.txt"

rango = [1] + list(range(5, 20, 5))
modo = ["big", "red"]
PCAS = ["PCA", "PCA_pesos"]

def leer_memoria_usada(path):
    res = []
    with open(path) as f:
        d = f.readlines()
    for i in d:
        res.append(float(i[:-1])/(1024*1024))
    return res

# Grafico entre PCA y PCA_pesos (ambos usando truco) para verios k de PCA

#for m in modo:
#    for k in rango:
#        datos = []
#        labels = []
#        for p in PCAS:
#            archivo_in = path_in.format(m, p, k)
#            datos.append(leer_memoria_usada(archivo_in))
#            labels.append(label.format(p, k))
#        grafico_boxplot(datos, labels)

for m in modo:
    archivo_in = path_in.format(m, "PCA", 1)
    archivo_in_sin_truco = path_in_sin_truco.format(m, "PCA", 1)

    datos = [leer_memoria_usada(archivo_in), leer_memoria_usada(archivo_in_sin_truco)]
    #labels = ["PCA + kNN, usando el truco", "PCA + kNN, sin usar el truco"]
    #if m == "big":
        #titulo = "Memoria utilizada según método para las imágenes originales"
    #else:
        #titulo = "Memoria utilizada según método para las imágenes reducidas"
    #grafico_boxplot(datos, labels, titulo, "Método","Memoria(Mb)") 
    for i in datos:
        df = pd.DataFrame(i)
        print(df.describe())
