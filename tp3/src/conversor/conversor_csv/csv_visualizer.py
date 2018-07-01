from PIL import Image
import numpy as np
import sys
import os
import csv
import scipy.misc

if len(sys.argv) < 2:
    print("Correr python converter.py <ruta_imagen_csv>.")
    print("Por ejemplo: python converter.py imagenes/tomo.csv")
    exit(0)

path_imagen = sys.argv[1]
imagen_numpy = np.genfromtxt(path_imagen, delimiter=',')
imagen = scipy.misc.toimage(imagen_numpy, cmin=0.0)
print("Imagen: " + path_imagen)
print("Dimensiones: " + str(imagen.size))
imagen.show()