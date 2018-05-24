import os
from PIL import Image

cant_imagenes_x_hilera = 5
carpeta = "92x112/"
tamaño_x = 92 * cant_imagenes_x_hilera
archivos = ["barbara_92x112.pgm", "buffalo_92x112.pgm", "coins_92x112.pgm", "columns_92x112.pgm", "dewey_defeats_truman_92x112.pgm", "ladyzhenskaya_92x112.pgm", "lena_92x112.pgm", "milking_92x112.pgm", "mona_lisa_92x112.pgm", "pepper_92x112.pgm", "snap_92x112.pgm", "sujeto16_2_92x112.pgm", "sujeto25_1_92x112.pgm", "surf_92x112.pgm", "cara1.pgm", "cara2.pgm", "cara3.pgm", "cara4.pgm", "cara5.pgm", "caraAA92x112.pgm", "caraCC92x112.pgm", "caraDD92x112.pgm", "EMILIANO92x112.pgm", "caraEE92x112.pgm", "caraFF92x112.pgm", "caraGG92x112.pgm", "caraHH92x112.pgm", "caraII92x112.pgm"]
imagenes = [Image.new('L', (tamaño_x, 112), color=255) for i in range(0, 6)]

j = 1
img = 0
for i in archivos:
    temp = Image.open(carpeta + i)
    rect = ((j-1)*92, 0, j*92, 112)
    imagenes[img].paste(temp, rect)
    if j % cant_imagenes_x_hilera == 0:
        j = 1
        img += 1
    else:
        j += 1

for i in range(0, len(imagenes)):
    imagenes[i].save("agrupada_{}.png".format(i+1))
