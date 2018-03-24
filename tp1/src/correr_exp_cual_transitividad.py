import subprocess
import os
import contar_transitividad

if __name__ == "__main__":
    os.system("python metnum.py build")

    parametro_datos_entrada = "experimentos/cualitativos/RANKINGS/datos/por_fecha/"

    modos = [1, 2] #1 = CMM_CL, 2 = WP

    for modo in modos:
        if modo == 2: 
            parametro_transitividad_resultante = "experimentos/cualitativos/RANKINGS/transitividad_por_fecha/WP/"
            parametro_datos_salida = "experimentos/cualitativos/RANKINGS/resultados/por_fecha/WP/"
            parametros_equipos_ordenados = "experimentos/cualitativos/RANKINGS/equipos_por_fecha/WP/"
        else:
            parametro_datos_salida = "experimentos/cualitativos/RANKINGS/resultados/por_fecha/CMM/"
            parametros_equipos_ordenados = "experimentos/cualitativos/RANKINGS/equipos_por_fecha/CMM/"
            parametro_transitividad_resultante = "experimentos/cualitativos/RANKINGS/transitividad_por_fecha/CMM/"

        archivos_entrada = os.listdir(parametro_datos_entrada)
        archivos_entrada.sort()

        for in_path in archivos_entrada:
            full_in_path = parametro_datos_entrada + in_path
            full_out_path = parametro_datos_salida + in_path.replace(".dat", ".out")
            full_equipos_path = parametros_equipos_ordenados + in_path.replace(".dat", ".equipos")
            full_transitividad_path = parametro_transitividad_resultante + in_path.replace(".dat", ".transitividad")

            programa = "./tp " + full_in_path + " " +\
                        full_out_path + " " +\
                        str(modo) + " 0 " + "mostrar_equipos"

            print("Corriendo " + programa)

            out = subprocess.getoutput(programa)

            with open(full_equipos_path, mode='w') as f:
                f.write(out)

            if modo == 2:
                contar_transitividad.contar_transitividad(full_in_path, full_out_path, full_equipos_path, full_transitividad_path, False)
            else:
                contar_transitividad.contar_transitividad(full_in_path, full_out_path, full_equipos_path, full_transitividad_path)

