from experimento_cualitativo_estrategia import leer_liga, leer_equipos, leer_ratings
from numpy import * 
arreglo = array([(equipos_resultado[i], ratings_resultado[i]) for i in range(0, 160000)])
i = 0
temp = 0.0
suma =0.0
while(i < 2):
	temp = porcentajes[i]
	suma = suma + temp
	i = i+1

print(suma/2)