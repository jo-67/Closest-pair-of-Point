"# Closest-pair-of-Point" 
## Compilacion y ejecucion
Fue realizado con la extencion de vsCode C/C++.

Se deben compilar y ejecutar los archivos: experiment_aleatorizado.c y experiment_points.c

# Metodos
En el codigo se implementan 5 metodos para encontrar el par de puntos mas cercanos

### Metodo Divide and Conquer
Se encuentra en source.c y corresponde a la funcion closestPairDivide

### Metodo Sweep Line
Se encuentra en source.c y corresponde a la funcion closestPairSweep

### Metodo Aleatorizado : Hash Universal
Se encuentra en aleatorizado.c y corresponde a la funcion closestPairAleatorizadoUniversal

### Metodo Aleatorizado : Funciones mas rapidas
Se encuentra en aleatorizado.c y corresponde a la funcion closestPairAleatorizadoRapido

### Metodo Aleatorizado : Primos de Mersenne
Se encuentra en aleatorizado.c y corresponde a la funcion closestPairAleatorizadoPrimos

# Experimentacion

### Comparacion de metodos Aleatorizados
Se realiza en experiment_aleatorizado.c y compara los 3 metodos aleatorizados 

### Comparacion de metodos Deterministicos vs Aleatorizados
Se realiza en experiment_points.c y compara los metodos de Divide and Conquer, Sweep Line y el mejor de los aleatorizados segun el experimento anterior