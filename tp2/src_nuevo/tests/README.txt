Para ayudar con la validación del código desarrollado se proveen cuatro casos de test: Big, FullBig, Red, FullRed.
El objetivo de estos casos de prueba es validar el cálculo de los autovalores/autovectores de la matriz de covarianza Mx = Xt.X especificada en el enunciado.
Cada caso de prueba X consta de dos archivos uno de entrada testX.in y uno de salida testX.expected.

testX.in:
	- Contiene las fotos a considerar en el set de entrenamiento con sus respectivas etiquetas en el formato especificado por la cátedra:
		<archivo1>, id1,
		...
		<archivoN>, idN,

testX.expected:
	- Contiene las raíces de los 15 autovalores de mayor magnitud de la matriz de covarianza Mx (v1, v2, ..., v15) ordenados decrecientemente.
		<v1>
		...
		<v15>

Aclaración: Para los archivos de entrada se asume que las fotos de tamaño completo se encuentran en un directorio llamado "full/" y las de tamaño reducido en uno llamado "reduced/".