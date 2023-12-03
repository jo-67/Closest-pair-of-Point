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

    if (n <= 2) {
        return DBL_MAX;
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
