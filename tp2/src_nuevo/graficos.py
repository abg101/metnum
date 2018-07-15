import matplotlib
matplotlib.use('GTK3Cairo')
import matplotlib.pyplot as plt
import numpy as np

matplotlib.rcParams.update({'font.size':18})

def grafico_linea(datos_x, datos_y, texto_x, texto_y, texto_gral, max_y_lim=100, modo='r', ancho=1, path=""):
    plt.plot(datos_x, datos_y, modo, linewidth=ancho)
    plt.xlabel(texto_x)
    plt.ylabel(texto_y)
    plt.title(texto_gral)
    plt.ylim((0, max_y_lim))
    if path == "":
        plt.show()
    else:
        plt.savefig(path)


# Asume que datos_y es una lista de listas, leyendas es lista de strings y modo es lista de strings
def grafico_lineas(datos_x, datos_y, texto_x, texto_y, texto_gral, leyendas, modos, max_y_lim=100, ancho=1, path=""):
    fig = plt.figure(figsize=(12,8))
    for i in range(0, len(datos_y)):
        plt.plot(datos_x, datos_y[i], modos[i], linewidth=ancho, label=leyendas[i])
    plt.xlabel(texto_x)
    plt.ylabel(texto_y)
    plt.title(texto_gral)
    plt.ylim((0, max_y_lim))
    plt.xticks(datos_x)
    if max_y_lim == 100:
        plt.yticks(list(range(0, 110, 10)))
    #plt.legend(bbox_to_anchor=(1.02, 1.00), loc=2, borderaxespad=0.)
    #plt.legend(loc='upper left')
    plt.legend(loc='lower right')
    if path == "":
        plt.show()
    else:
        plt.savefig(path, bbox_inches='tight')
    plt.close(fig)

# Asume que datos_y es una lista de diccionarios, leyendas es lista de strings, modo es lista de strings
# Max_y_lim es lista de enteros
# Texto_y es lista de strings
def grafico_lineas_varios(datos_x, datos_y, texto_x, texto_y, texto_gral, leyendas, modos, plot_dim, max_y_lim, ancho=1, path=""):
    fig = plt.figure(figsize=(20,10))
    for i in range(0, len(datos_y)):
        plt.subplot(plot_dim + i + 1)
        for c in range(0, len(datos_y[i])):
            plt.xlabel(texto_x)
            plt.ylabel(texto_y[i])
            #plt.title(texto_gral)
            plt.ylim((0, max_y_lim[i]))
            plt.xticks(datos_x)
            if max_y_lim[i] == 100:
                plt.yticks(list(range(0, 110, 10)))
            plt.plot(datos_x, datos_y[i][c], modos[c], linewidth=ancho, label=leyendas[c])
    plt.legend(bbox_to_anchor=(1.05, 1.05), loc=2, borderaxespad=0.)
    plt.subplots_adjust(top=0.92, bottom=0.08, left=0.10, right=0.95, 
            hspace=0.25, wspace=0.35)
    if path == "":
        plt.show()
    else:
        plt.savefig(path)
    plt.close(fig)

# Asume que datos_y es una lista de listas, leyendas es lista de strings y modo es lista de strings
def grafico_stack_plot(datos_x, datos_y, texto_x, texto_y, texto_gral, leyendas, modos, max_y_lim=100, ancho=1, path=""):
    plt.stackplot(datos_x, datos_y, colors=modos, label=leyendas, linewidth=ancho)
    plt.xlabel(texto_x)
    plt.ylabel(texto_y)
    plt.title(texto_gral)
    plt.ylim((0, max_y_lim))
    plt.xticks(datos_x)
    if max_y_lim == 100:
        plt.yticks(list(range(0, 110, 10)))
    plt.legend()
    if path == "":
        plt.show()
    else:
        plt.savefig(path)


def grafico_boxplot(datos, labels_data, titulo, xlab, ylab, path=""):
    fig = plt.figure()
    plt.boxplot(datos, labels=labels_data)
    plt.title(titulo)
    plt.xlabel(xlab)
    plt.ylabel(ylab)
    if path == "":
        plt.show()
    else:
        plt.savefig(path)
    plt.close(fig)


def grafico_barras(datos, xlabels, ylabel, titulo, colors, path=""):
    fig, ax = plt.subplots()
    ind = np.arange(len(xlabels))
    width = 0.20
    rects = list()

    espacio = 0
    for i in sorted(datos.keys()):
        rect = ax.bar(ind + width*espacio, datos[i], width, color=colors[espacio])
        espacio += 1
        rects.append(rect)

    ax.set_ylabel(ylabel)
    ax.set_title(titulo)
    ax.set_xticks(ind + width / 2)
    ax.set_xticklabels(xlabels)
    ax.set_yticks(range(0, 110, 10))
    
    ax.legend([r[0] for r in rects], sorted(datos.keys()), loc='upper center', fancybox=True, ncol=3, bbox_to_anchor=(0.5, -0.05))

    if path == "":
        plt.show()
    else:
        plt.savefig(path)
    plt.close()


