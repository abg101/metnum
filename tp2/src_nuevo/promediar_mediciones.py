import sys
#from numpy import *
from random import shuffle
import os

# Como usar
# python3 promediar_mediciones.py [K de kfold]  
# K de kfold debe estar 5
# metodo debe ser PCA o PCA_pesos

k_folds = int(sys.argv[1])
#metodo = sys.argv[2] #knn - pca
modos = ["big", "red"]
knn = [1,3,5,7] #int(sys.argv[3])
path_resumen_sinPCA = "experimentos/experimento_1/promedio_mediciones_{0}/knn_{4}/promedio_{0}_{1}_K{2}_fold{3}.txt"
path_resumen_conPCA = "experimentos/experimento_1/promedio_mediciones_{0}/knn_{5}/pca_{3}/promedio_{0}_{1}_K{2}_alpha{3}_fold{4}.txt"
#sizes = [20,50,75,100]
alphas = [0,1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]
for m in modos:    
    for k in knn:  

        Promedio_Tiempo = dict(zip([0],[0]))
        Promedio_Accuracy_general = dict(zip([0],[0]))
        Promedio_Precision_general = dict(zip([0],[0]))
        Promedio_Recall_general = dict(zip([0],[0]))
        Promedio_Accuracy_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)])) 
        Promedio_Precision_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
        Promedio_Recall_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
        for a in alphas:
            for j in range(1,k_folds + 1):
                path = "experimentos/experimento_1/mediciones_{0}/knn_{3}/{0}_size{4}_K{1}_knn{3}_alpha{5}_fold{2}.csv.medidas".format(m, k_folds, j, knn,100,a)
                if not os.path.isfile(path):
                    path = "experimentos/experimento_1/mediciones_{0}/knn_{3}/{0}_size{4}_K{1}_knn{3}_fold{2}.csv.medidas".format(m, k_folds, j, knn,100)
                    if not os.path.isfile(path):
                        break


                with open(path, mode='r') as fr:
                    fr.readline()
                    temp = Promedio_Tiempo[0]
                    Promedio_Tiempo[0] = temp + float(fr.readline())
                    fr.readline()
                    temp = Promedio_Accuracy_general[0]
                    Promedio_Accuracy_general[0] = temp + float(fr.readline())
                    fr.readline()
                    temp = Promedio_Precision_general[0]
                    Promedio_Precision_general[0] = temp + float(fr.readline())
                    fr.readline()
                    temp = Promedio_Recall_general[0]
                    Promedio_Recall_general[0] = temp + float(fr.readline())
                    lineaActual = fr.readline() #Accuracy por sujeto
                    lineaActual = fr.readline()
                    l = lineaActual.split(": ")
                    while (l[0] != "Precision por sujeto\n"):
                        clave = int(l[0])
                        significado = float(l[1])
                        temp = Promedio_Accuracy_sujeto[clave]
                        Promedio_Accuracy_sujeto[clave] = temp + significado 
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
                if a == 0:
                    with open(path_resumen_sinPCA.format(m,"knn",k_folds,j,knn), mode='w') as fw:
                        fw.write("Promedio de Tiempos(en ticks):\n")  
                        fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
                        fw.write("Promedio Accuracy general:\n")  
                        fw.write("{}\n".format(Promedio_Accuracy_general[0]/k_folds))
                        fw.write("Promedio Precision general:\n")  
                        fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
                        fw.write("Promedio Recall general:\n")  
                        fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

                        fw.write("Promedio Accuracy por sujeto:\n")
                        for k in Promedio_Accuracy_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Accuracy_sujeto[k]/k_folds))

                        fw.write("Promedio Precision por sujeto:\n")
                        for k in Promedio_Precision_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

                        fw.write("Promedio Recall por sujeto:\n")
                        for k in Promedio_Recall_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))
                else:        
                    with open(path_resumen_conPCA.format(m,"pca",k_folds, a,j,knn), mode='w') as fw:
                        fw.write("Promedio de Tiempos(en ticks):\n")  
                        fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
                        fw.write("Promedio Accuracy general:\n")  
                        fw.write("{}\n".format(Promedio_Accuracy_general[0]/k_folds))
                        fw.write("Promedio Precision general:\n")  
                        fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
                        fw.write("Promedio Recall general:\n")  
                        fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

                        fw.write("Promedio Accuracy por sujeto:\n")
                        for k in Promedio_Accuracy_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Accuracy_sujeto[k]/k_folds))

                        fw.write("Promedio Precision por sujeto:\n")
                        for k in Promedio_Precision_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

                        fw.write("Promedio Recall por sujeto:\n")
                        for k in Promedio_Recall_sujeto.keys():
                            fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))
                    

"""
                #casos k5-k100

                    Promedio_Tiempo = dict(zip([0],[0]))
                    Promedio_Accuracy_general = dict(zip([0],[0]))
                    Promedio_Precision_general = dict(zip([0],[0]))
                    Promedio_Recall_general = dict(zip([0],[0]))
                    Promedio_Accuracy_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)])) 
                    Promedio_Precision_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
                    Promedio_Recall_sujeto = dict(zip([i for i in range(1, 42)],[0 for i in range(1, 42)]))
                    
                    for j in range(1,k_folds + 1):
                        path = "experimentos/experimento_1/mediciones_{0}/knn_{4}/{0}_size{5}_K{2}_knn{4}_alpha{6}_fold{3}.csv.medidas".format(m, metodo, k_folds, j, knn,100,a)
                        if not os.path.isfile(path):
                            path = "experimentos/experimento_1/mediciones_{0}/knn_{4}/{0}_size{5}_K{2}_knn{4}_fold{3}.csv.medidas".format(m, metodo, k_folds, j, knn,100)
                            if not os.path.isfile(path):
                                break

                        with open(path, mode='r') as fr:
                            fr.readline()
                            temp = Promedio_Tiempo[0]
                            Promedio_Tiempo[0] = temp + float(fr.readline())
                            fr.readline()
                            temp = Promedio_Accuracy_general[0]
                            Promedio_Accuracy_general[0] = temp + float(fr.readline())
                            fr.readline()
                            temp = Promedio_Precision_general[0]
                            Promedio_Precision_general[0] = temp + float(fr.readline())
                            fr.readline()
                            temp = Promedio_Recall_general[0]
                            Promedio_Recall_general[0] = temp + float(fr.readline())
                            lineaActual = fr.readline() #Accuracy por sujeto
                            lineaActual = fr.readline()
                            l = lineaActual.split(": ")
                            while (l[0] != "Precision por sujeto\n"):
                                clave = int(l[0])
                                significado = float(l[1])
                                temp = Promedio_Accuracy_sujeto[clave]
                                Promedio_Accuracy_sujeto[clave] = temp + significado 
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
                                

                    if a == 0:
                        with open(path_resumen_sinPCA.format(m,metodo,k_folds, j,knn,100), mode='w') as fw:
                            fw.write("Promedio de Tiempos(en ticks):\n")
                            fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
                            fw.write("Promedio Accuracy general:\n")  
                            fw.write("{}\n".format(Promedio_Accuracy_general[0]/k_folds))
                            fw.write("Promedio Precision general:\n")  
                            fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
                            fw.write("Promedio Recall general:\n")  
                            fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

                            fw.write("Promedio Accuracy por sujeto:\n")
                            for k in Promedio_Accuracy_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Accuracy_sujeto[k]/k_folds))

                            fw.write("Promedio Precision por sujeto:\n")
                            for k in Promedio_Precision_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

                            fw.write("Promedio Recall por sujeto:\n")
                            for k in Promedio_Recall_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))

                    else: 
                        with open(path_resumen_conPCA.format(m,metodo,k_folds, a,j,knn,100,a), mode='w') as fw:
                            fw.write("Promedio de Tiempos(en ticks):\n")
                            fw.write("{}\n".format(Promedio_Tiempo[0]/k_folds))
                            fw.write("Promedio Accuracy general:\n")  
                            fw.write("{}\n".format(Promedio_Accuracy_general[0]/k_folds))
                            fw.write("Promedio Precision general:\n")  
                            fw.write("{}\n".format(Promedio_Precision_general[0]/k_folds))
                            fw.write("Promedio Recall general:\n")  
                            fw.write("{}\n".format(Promedio_Recall_general[0]/k_folds))

                            fw.write("Promedio Accuracy por sujeto:\n")
                            for k in Promedio_Accuracy_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Accuracy_sujeto[k]/k_folds))

                            fw.write("Promedio Precision por sujeto:\n")
                            for k in Promedio_Precision_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Precision_sujeto[k]/k_folds))

                            fw.write("Promedio Recall por sujeto:\n")
                            for k in Promedio_Recall_sujeto.keys():
                                fw.write("{0}: {1}\n".format(k,Promedio_Recall_sujeto[k]/k_folds))
"""