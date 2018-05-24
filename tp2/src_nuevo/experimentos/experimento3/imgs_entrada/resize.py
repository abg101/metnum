from PIL import Image
import os

path = "originales/"

archivos = os.listdir(path)

tamaños = [(92, 112), (23, 28)]

for i in archivos:
    im = Image.open(path + i)
    for t in tamaños:
        re = im.resize(t)
        re.save(i[:len(i)-4] + "_"+ str(t[0]) + "x" + str(t[1]) + ".pgm")
