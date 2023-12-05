#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

typedef struct Punto {
    double x;
    double y;
} Punto;
double distance(Punto p1, Punto p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

typedef struct Node {
    Punto p;
    struct Node* siguiente;
} Node;


// numero random entero entre 0 y n
int randomNumber(int n) {
    int numeroAleatorio = rand();
    numeroAleatorio = numeroAleatorio % (n + 1);
    return numeroAleatorio;
}
// crea una grilla dxd
// [[Node_0*....d], [0....d], [],.... d[0 .... d] ]
Node*** createGrid(int n) {
    Node*** arregloDeArreglos = (Node***)malloc(n * sizeof(Node**));

    // Inicializar cada arreglo en el arreglo principal
    for (int i = 0; i < n; i++) {
        arregloDeArreglos[i] = (Node**)malloc(n * sizeof(Node*));
        for (int j=0; j< n; j++) {
            arregloDeArreglos[i][j] = NULL;
        }
    }
    return arregloDeArreglos;
}

void freeGrid(Node*** grid, int n) {
    for (int i = 0; i < n; i++) {
        for (int j=0; j< n; j++) {
            Node* aux = grid[i][j];
            while(aux != NULL) {
                Node* aux2 = aux;
                aux = aux->siguiente;
                free(aux2);
            }
        }
        free(grid[i]);
    }
    free(grid);
}

int hashUniversal(int x, int a, int b, int p, int m) {
    return ((int)a * x + b) % p % m;
}
int hashRapido(int x, int a, int b, int k, int l) {
    int result = (a * x + b) & ((1 << k) - 1);
    return result >> l;
}
int hashPrimosMersenne(int x, int m, int l) {
    // (x << m) - x == x * ((2**m) - 1)
    return ((x << m) - x)%l;
}
double closestPairAleatorizadoUniversal(Punto* points, int n) {
    double d = DBL_MAX;
    // se seleccionan n pares y se calcula el menor d
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        int j = randomNumber(n);
        int k = randomNumber(n);
        while(k==j) { // elimina pares iguales
            k = randomNumber(n);
        }
        double dis = distance(points[j], points[k]);
        if (dis < d) {
            d = dis;
        }
    }
    //printf("random d = %f\n", d);
    int not_d = (int) 1/d;
    not_d++;
    //int l = (int) log2(not_d);
    //int m = 1 << (l + 1);
    Node*** grid = createGrid(not_d);
    int d_grid = d;
    // poner los puntos en la grilla dxd}
    int a = randomNumber(n);
    int b = randomNumber(n);
    int p = 100000007; // maximo primo representable
    for (int i = 0; i < n; i++) {
        int x = hashUniversal(points[i].x, a, b, p, not_d);
        int y = hashUniversal(points[i].y, a, b, p, not_d);
        
        Node* nodo = (Node*)malloc(sizeof(Node));
        nodo->p = points[i];
        nodo->siguiente = NULL;
        if (grid[x][y] == NULL) {
            grid[x][y] = nodo;
        } else {
            Node* aux = grid[x][y];
            while(aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nodo;
        }
    }
    // Revisar casilla y vecinos
    for (int i = 0; i < n; i++) {
        int x = hashUniversal(points[i].x, a, b, p, not_d);
        int y = hashUniversal(points[i].y, a, b, p, not_d);
        for (int j = x-1; j <= x+1; j++) {
            for (int k = y-1; k <= y+1; k++) {
                if (j >= 0 && j < d && k >= 0 && k < d) {
                    Node* aux = grid[j][k];
                    while(aux != NULL) {
                        double dis = distance(points[i], aux->p);
                        if (dis < d) {
                            d = dis;
                        }
                        aux = aux->siguiente;
                    }
                }
            }
        }
    }

    freeGrid(grid, not_d);
    return d;

}
double closestPairAleatorizadoRapido(Punto* points, int n) {
    double d = DBL_MAX;
    // se seleccionan n pares y se calcula el menor d
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        int j = randomNumber(n);
        int k = randomNumber(n);
        while(k==j) { // elimina pares iguales
            k = randomNumber(n);
        }
        double dis = distance(points[j], points[k]);
        if (dis < d) {
            d = dis;
        }
    }
    //printf("random d = %f\n", d);
    int not_d = (int) 1/d;
    not_d++;
    int l = (int) log2(not_d);
    l++;
    int km = 2*l;
    int m = 1 << (km-l);
    Node*** grid = createGrid(m);
    int d_grid = d;
    // poner los puntos en la grilla dxd}
    int a = randomNumber(n);
    int b = randomNumber(n);
    for (int i = 0; i < n; i++) {
        int x = hashRapido(points[i].x, a, b, km, l);
        int y = hashRapido(points[i].y, a, b, km, l);
        
        Node* nodo = (Node*)malloc(sizeof(Node));
        nodo->p = points[i];
        nodo->siguiente = NULL;
        if (grid[x][y] == NULL) {
            grid[x][y] = nodo;
        } else {
            Node* aux = grid[x][y];
            while(aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nodo;
        }
    }
    // Revisar casilla y vecinos
    for (int i = 0; i < n; i++) {
        int x = hashRapido(points[i].x, a, b, km, l);
        int y = hashRapido(points[i].y, a, b, km, l);
        for (int j = x-1; j <= x+1; j++) {
            for (int k = y-1; k <= y+1; k++) {
                if (j >= 0 && j < d && k >= 0 && k < d) {
                    Node* aux = grid[j][k];
                    while(aux != NULL) {
                        double dis = distance(points[i], aux->p);
                        if (dis < d) {
                            d = dis;
                        }
                        aux = aux->siguiente;
                    }
                }
            }
        }
    }

    freeGrid(grid, m);
    return d;

}

double closestPairAleatorizadoPrimos(Punto* points, int n) {
    double d = DBL_MAX;
    // se seleccionan n pares y se calcula el menor d
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        int j = randomNumber(n);
        int k = randomNumber(n);
        while(k==j) { // elimina pares iguales
            k = randomNumber(n);
        }
        double dis = distance(points[j], points[k]);
        if (dis < d) {
            d = dis;
        }
    }
    //printf("random d = %f\n", d);
    int not_d = (int) 1/d;
    not_d++;
    int l = (int) log2(not_d);
    l++;
    int km = 2*l;
    int m = 1 << (km-l);
    Node*** grid = createGrid(m);
    int d_grid = d;
    // poner los puntos en la grilla dxd}
    int a = randomNumber(n);
    int b = randomNumber(n);
    for (int i = 0; i < n; i++) {
        int x = hashPrimosMersenne(points[i].x, l, m);
        int y = hashPrimosMersenne(points[i].y, l, m);
        
        Node* nodo = (Node*)malloc(sizeof(Node));
        nodo->p = points[i];
        nodo->siguiente = NULL;
        if (grid[x][y] == NULL) {
            grid[x][y] = nodo;
        } else {
            Node* aux = grid[x][y];
            while(aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nodo;
        }
    }
    // Revisar casilla y vecinos
    for (int i = 0; i < n; i++) {
        int x = hashPrimosMersenne(points[i].x, l, m);
        int y = hashPrimosMersenne(points[i].y, l, m);
        for (int j = x-1; j <= x+1; j++) {
            for (int k = y-1; k <= y+1; k++) {
                if (j >= 0 && j < d && k >= 0 && k < d) {
                    Node* aux = grid[j][k];
                    while(aux != NULL) {
                        double dis = distance(points[i], aux->p);
                        if (dis < d) {
                            d = dis;
                        }
                        aux = aux->siguiente;
                    }
                }
            }
        }
    }

    freeGrid(grid, m);
    return d;

}