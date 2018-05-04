import sys
#from numpy import *
from random import shuffle
import os

# Como usar
# python3 promediar_mediciones.py [K de kfold] [metodo] [knn]
# K de kfold debe estar 5
# metodo debe ser PCA o PCA_pesos
# knn debe ser 1,3 o 7 . El 5 ya está hecho y de distinta forma


k_folds = int(sys.argv[1])
metodo = sys.argv[2] #PCA - PCA_pesos
modos = ["big", "red"]
knn = int(sys.argv[3])
path_resumen = "experimentos/experimento 1/promedio_mediciones_{0}/knn_{5}/promedio_{0}_{1}_K{2}_k{3}_fold{4}.txt"

for m in modos:    
    
    #caso k1
    Promedio_Tiempo = dict(zip([0],[0]))
    Promedio_Hitrate_general = dict(zip([0],[0]))
    Promedio_Precision_general = dict(zip([0],[0]))
    Promedio_Recall_general = dict(zip([0],[0]))
    Promedio_Hitrate_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)])) 
    Promedio_Precision_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
    Promedio_Recall_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
    for j in range(1,k_folds + 1):

        path = "experimentos/experimento 1/mediciones_{0}/knn_{4}/{1}_1/{0}_{1}_K{2}_k1_fold{3}.out.medidas".format(m, metodo, k_folds, j, knn)
        if not os.path.isfile(path):
            path = "experimentos/experimento 1/mediciones_{0}/knn_{4}/{1}_1/{0}_K{2}_k1_fold{3}.out.medidas".format(m, metodo, k_folds, j, knn)



        with open(path, mode='r') as fr:
            fr.readline()
            temp = Promedio_Tiempo[0]
            Promedio_Tiempo[0] = temp + float(fr.readline())
            fr.readline()
            temp = Promedio_Hitrate_general[0]
            Promedio_Hitrate_general[0] = temp + float(fr.readline())
            fr.readline()
            temp = Promedio_Precision_general[0]
            Promedio_Precision_general[0] = temp + float(fr.readline())
            fr.readline()
            temp = Promedio_Recall_general[0]
            Promedio_Recall_general[0] = temp + float(fr.readline())
            lineaActual = fr.readline() #Hitrate por sujeto
            lineaActual = fr.readline()
            l = lineaActual.split(": ")
            while (l[0] != "Precision por sujeto\n"):
                clave = int(l[0])
                significado = float(l[1])
                temp = Promedio_Hitrate_sujeto[clave]
                Promedio_Hitrate_sujeto[clave] = temp + significado 
                lineaActual = fr.readline()
                l = lineaActual.split(": ")
            
            lineaActual = fr.readline()
            l = lineaActual.split(": ")
            while (l[0] != "Recall por sujeto\n"):
                clave = int(l[0])
                significado = float(l[1])
                temp = Promedio_Precision_sujeto[clave]
                Promedio_Precision_sujeto[clave] = temp + significado 
                lineaActual = fr.readline()
                l = lineaActual.split(": ")

            lineas = fr.readlines()
            for k in lineas:
                lineaActual = k
                l = lineaActual.split(": ")
                clave = int(l[0])
                significado = float(l[1])
                temp = Promedio_Recall_sujeto[clave]
                Promedio_Recall_sujeto[clave] = temp + significado 


    with open(path_resumen.format(m,metodo,k_folds, 1,j,knn), mode='w') as fw:
        fw.write("Promedio de Tiempos(en ticks):\n")  
        fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
        fw.write("Promedio Hitrate general:\n")  
        fw.write("{}\n".format(Promedio_Hitrate_general[0]/k_folds))
        fw.write("Promedio Precision general:\n")  
        fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
        fw.write("Promedio Recall general:\n")  
        fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

        fw.write("Promedio Hitrate por sujeto:\n")
        for k in Promedio_Hitrate_sujeto.keys():
            fw.write("{0}: {1}\n".format(k,Promedio_Hitrate_sujeto[k]/k_folds))

        fw.write("Promedio Precision por sujeto:\n")
        for k in Promedio_Precision_sujeto.keys():
            fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

        fw.write("Promedio Recall por sujeto:\n")
        for k in Promedio_Recall_sujeto.keys():
            fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))
        


    #casos k5-k100
    for i in range(5, 105, 5):

        Promedio_Tiempo = dict(zip([0],[0]))
        Promedio_Hitrate_general = dict(zip([0],[0]))
        Promedio_Precision_general = dict(zip([0],[0]))
        Promedio_Recall_general = dict(zip([0],[0]))
        Promedio_Hitrate_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)])) 
        Promedio_Precision_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
        Promedio_Recall_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
        
        for j in range(1,k_folds + 1):
            path = "experimentos/experimento 1/mediciones_{0}/knn_{5}/{1}_{3}/{0}_{1}_K{2}_k{3}_fold{4}.out.medidas".format(m, metodo, k_folds, i, j, knn)
            if not os.path.isfile(path):
                path = "experimentos/experimento 1/mediciones_{0}/knn_{5}/{1}_{3}/{0}_K{2}_k{3}_fold{4}.out.medidas".format(m, metodo, k_folds, i, j, knn)


            with open(path, mode='r') as fr:
                fr.readline()
                temp = Promedio_Tiempo[0]
                Promedio_Tiempo[0] = temp + float(fr.readline())
                fr.readline()
                temp = Promedio_Hitrate_general[0]
                Promedio_Hitrate_general[0] = temp + float(fr.readline())
                fr.readline()
                temp = Promedio_Precision_general[0]
                Promedio_Precision_general[0] = temp + float(fr.readline())
                fr.readline()
                temp = Promedio_Recall_general[0]
                Promedio_Recall_general[0] = temp + float(fr.readline())
                lineaActual = fr.readline() #Hitrate por sujeto
                lineaActual = fr.readline()
                l = lineaActual.split(": ")
                while (l[0] != "Precision por sujeto\n"):
                    clave = int(l[0])
                    significado = float(l[1])
                    temp = Promedio_Hitrate_sujeto[clave]
                    Promedio_Hitrate_sujeto[clave] = temp + significado 
                    lineaActual = fr.readline()
                    l = lineaActual.split(": ")
                
                lineaActual = fr.readline()
                l = lineaActual.split(": ")
                while (l[0] != "Recall por sujeto\n"):
                    clave = int(l[0])
                    significado = float(l[1])
                    temp = Promedio_Precision_sujeto[clave]
                    Promedio_Precision_sujeto[clave] = temp + significado 
                    lineaActual = fr.readline()
                    l = lineaActual.split(": ")

                lineas = fr.readlines()
                for k in lineas:
                    lineaActual = k
                    l = lineaActual.split(": ")
                    clave = int(l[0])
                    significado = float(l[1])
                    temp = Promedio_Recall_sujeto[clave]
                    Promedio_Recall_sujeto[clave] = temp + significado 
                    


        with open(path_resumen.format(m,metodo,k_folds,i,j,knn), mode='w') as fw:
            fw.write("Promedio de Tiempos(en ticks):\n")
            fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
            fw.write("Promedio Hitrate general:\n")  
            fw.write("{}\n".format(Promedio_Hitrate_general[0]/k_folds))
            fw.write("Promedio Precision general:\n")  
            fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
            fw.write("Promedio Recall general:\n")  
            fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

            fw.write("Promedio Hitrate por sujeto:\n")
            for k in Promedio_Hitrate_sujeto.keys():
                fw.write("{0}: {1}\n".format(k,Promedio_Hitrate_sujeto[k]/k_folds))

            fw.write("Promedio Precision por sujeto:\n")
            for k in Promedio_Precision_sujeto.keys():
                fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

            fw.write("Promedio Recall por sujeto:\n")
            for k in Promedio_Recall_sujeto.keys():
                fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))
