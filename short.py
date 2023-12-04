import pandas as pd
import re

# Definir un diccionario para almacenar los resultados por universo
resultados = {}

# Leer los resultados desde el archivo (puedes ajustar el nombre del archivo)
with open('resultados.txt', 'r') as archivo:
    # Utiliza read() para leer todo el contenido como una sola cadena
    resultados_texto = archivo.read()

exe = resultados_texto.split("Ejecutando para ")
#Ejecutando para 2^1 
#RadixSort tom├│ 6.07 segundos en ejecutarse.
#QuickSort tom├│ 5.29 segundos en ejecutarse.
#End
divide = []
sweep = []
aleatorio = []
for i in exe:
    mod = i.replace(" tomó ", " ")
    mod = mod.replace(" milisegundos", "")
    ke = mod.split("\n")
    n = ke[0].replace("n=", "")
    for line in ke[1:]:
        if line != "" and line!="End":
            a = line.split(" ")
            r = a[0]
            t = a[2]
            try:
                t = float(t)
            except:
                t = 0
            if r=="Divide_and_Conquer":
                divide = divide + [[n, t]]
            elif r=="Sweep_line":
                sweep = sweep + [[n, t]] 
            else:
                aleatorio = aleatorio + [[n, t]]

#print(l)
dfd = pd.DataFrame(divide)
dfs = pd.DataFrame(sweep)
dfa = pd.DataFrame(aleatorio)

dfd.to_excel('resultadosDivide.xlsx', index=False)
dfs.to_excel('resultadosSweep.xlsx', index=False)
dfa.to_excel('resultadosAleatorio.xlsx', index=False)