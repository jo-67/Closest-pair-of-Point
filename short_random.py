import pandas as pd
import re

# Definir un diccionario para almacenar los resultados por universo
resultados = {}

# Leer los resultados desde el archivo (puedes ajustar el nombre del archivo)
with open('resultados_random.txt', 'r') as archivo:
    # Utiliza read() para leer todo el contenido como una sola cadena
    resultados_texto = archivo.read()

exe = resultados_texto.split("Ejecutando para ")
#Ejecutando para 2^1 
#RadixSort tom├│ 6.07 segundos en ejecutarse.
#QuickSort tom├│ 5.29 segundos en ejecutarse.
#End
universal = []
rapido = []
primos = []
for i in exe:
    mod = i.replace(" tom├│ ", " ")
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
            if r=="Hashing_universal":
                universal = universal + [[n, t]]
            elif r=="Rapida":
                rapido = rapido + [[n, t]] 
            else:
                primos = primos + [[n, t]]

#print(l)
dfd = pd.DataFrame(universal)
dfs = pd.DataFrame(rapido)
dfa = pd.DataFrame(primos)

dfd.to_excel('resultadosUniversal.xlsx', index=False)
dfs.to_excel('resultadosRapido.xlsx', index=False)
dfa.to_excel('resultadosPrimos.xlsx', index=False)