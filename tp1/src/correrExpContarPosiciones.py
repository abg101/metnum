import subprocess
import os
import contarPosiciones

if __name__ == "__main__":
    os.system("python metnum.py build")

    parametro_datos_entrada = "experimentos/cualitativos/RANKINGS/datos/por_fecha/"

    modos = [1, 2] #1 = CMM_CL, 2 = WP

    for modo in modos:
        if modo == 2: 
            parametro_posiciones_resultante = "experimentos/cualitativos/RANKINGS/posiciones_por_fecha/WP/"
            parametro_datos_salida = "experimentos/cualitativos/RANKINGS/resultados/por_fecha/WP/"
            parametros_equipos_ordenados = "experimentos/cualitativos/RANKINGS/equipos_por_fecha/WP/"
        else:
            parametro_datos_salida = "experimentos/cualitativos/RANKINGS/resultados/por_fecha/CMM/"
            parametros_equipos_ordenados = "experimentos/cualitativos/RANKINGS/equipos_por_fecha/CMM/"
            parametro_posiciones_resultante = "experimentos/cualitativos/RANKINGS/posiciones_por_fecha/CMM/"

        lista1 = os.listdir(parametro_datos_entrada)
        lista1.sort()
        lista2 = lista1[2:] #porque el primer elemento de la lista no tiene todos los equipos
        ultimo = len(lista1) - 1
        lista1 = lista1[1:ultimo]
            
        for i, j in zip(lista1, lista2):
            full_in_path_1 = parametro_datos_entrada + i
            full_out_path_1 = parametro_datos_salida + i.replace(".dat", ".out")
            full_equipos_path_1 = parametros_equipos_ordenados + i.replace(".dat", ".equipos")
            full_in_path_2 = parametro_datos_entrada + j
            full_out_path_2 = parametro_datos_salida + j.replace(".dat", ".out")
            full_equipos_path_2 = parametros_equipos_ordenados + j.replace(".dat", ".equipos")
            full_posiciones_path = parametro_posiciones_resultante + i.replace(".dat", ".Posiciones")
          #  programa = "./tp " + full_in_path + " " +\
           #             full_out_path + " " +\
            #            str(modo) + " 0 " + "mostrar_equipos"

           # print("Corriendo " + programa)

#            out = subprocess.getoutput(programa)

 #           with open(full_equipos_path, mode='w') as f:
  #              f.write(out)

            if modo == 2:
                contarPosiciones.contar_posiciones(full_in_path_1,full_in_path_2, full_out_path_1,full_out_path_2, full_equipos_path_1, full_equipos_path_2, full_posiciones_path, False)
            else:
                contarPosiciones.contar_posiciones(full_in_path_1,full_in_path_2, full_out_path_1,full_out_path_2, full_equipos_path_1, full_equipos_path_2, full_posiciones_path)



