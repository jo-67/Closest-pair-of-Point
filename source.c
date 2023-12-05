#include <float.h>
#include <stdlib.h>
typedef struct Punto {
    double x;
    double y;
} Punto;

int compareX(const void* a, const void* b) {
    return ((Punto*)a)->x - ((Punto*)b)->x;
}
int compareY(const void* a, const void* b) {
    return ((Punto*)a)->y - ((Punto*)b)->y;
}
double distance(Punto p1, Punto p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}
double closestPairSweep(Punto* points, int n) {
    if (n <= 1) {
        return DBL_MAX;
    }

    qsort(points, n, sizeof(Punto), compareX);
    double d = DBL_MAX;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n && (points[j].x - points[i].x) < d; j++) {
            double distancia = distance(points[i], points[j]);
            if (distancia < d) {
                d = distancia;
            }
        }
    }

    return d;
}

double stripClosest(Punto* strip, int j, double d) {
    double min = d;
    qsort(strip, j, sizeof(Punto), compareY);
    for (int i = 0; i < j; i++) {
        for (int k = i+1; k < j && fabs(strip[k].y - strip[i].y) < min; k++) {
            double distancia = distance(strip[i], strip[k]);
            if (distancia < min) {
                min = distancia;
            }
        }
    }
    return min;
}

double closestPairDivide(Punto* points, int n) {
    qsort(points, n, sizeof(Punto), compareX);

    if (n < 2) {
        return DBL_MAX;
    }
    if (n == 2) {
        return distance(points[0], points[1]);
    }

    int mid = n / 2;
    Punto midPoint = points[mid];
    double dl = closestPairDivide(points, mid);
    double dr = closestPairDivide(points + mid, n - mid);
    double d = fmin(dl, dr); 

    Punto* strip = malloc(n * sizeof(Punto));
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(points[i].x - midPoint.x) < d) {
            strip[j] = points[i];
            j++;
        }
    }

    double minDistance = fmin(d, stripClosest(strip, j, d));

    free(strip);
    return minDistance;
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

int hashPrimosMersenne(int x, int m, int l) {
    // (x << m) - x == x * ((2**m) - 1)
    return ((x << m) - x)%l;
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