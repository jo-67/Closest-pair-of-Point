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
    srand(time(NULL));
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

double closestPairAleatorizadoUniversal(Punto* points, int n) {
    double d = DBL_MAX;
    // se seleccionan n pares y se calcula el menor d
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
    Node*** grid = createGrid(d);
    int d_grid = d;
    // poner los puntos en la grilla dxd
    for (int i = 0; i < n; i++) {
        int x = hashUniversal(points[i].x, 3, 5, 7, d);
        int y = hashUniversal(points[i].y, 3, 5, 7, d);
        
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
        int x = hashUniversal(points[i].x, 3, 5, 7, d);
        int y = hashUniversal(points[i].y, 3, 5, 7, d);
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

    freeGrid(grid, d_grid);
    return d;

}