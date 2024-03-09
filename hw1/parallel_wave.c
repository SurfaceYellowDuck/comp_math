#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

#define N 10      // Размер сетки
#define MAX_ITER 1000 // Максимальное количество итераций
#define EPSILON 0.1

void init(double **u) {
    double h = 1.0 / (N + 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
//                u[i][j] = 1;
                if (i == 0) {
                    u[i][j] = 100 - 200 * h * j; // Граничное условие f(0, y) = 100 - 200y
                    if (j == N - 1) {
                        continue;
                    }
                }

                if (i == (N - 1)) {
                    u[i][j] = -100 + 200 * h * j; // Граничное условие f(1, y) = -100 + 200y
                }

                if (j == 0) {
                    u[i][j] = 100 - 200 * h * i; // Граничное условие f(x, 0) = 100 - 200x
                }

                if (j == (N - 1)) {
                    u[i][j] = -100 + 200 * h * i; // Граничное условие f(x, 1) = -100 + 200x
                }
            } else {
                int num = (rand() %
                           (100 - (-100) + 1)) + (-100);
                u[i][j] = num; // Внутри области D задаем случайное значение
            }
        }
    }
}

void init_f(double **f) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            f[i][j] = 0.0;
        }
    }
}

int main() {
    double **u = (double **) malloc(N * sizeof(double *));
    double **f = (double **) malloc(N * sizeof(double *));
//    double *dm = (double **) malloc(N * sizeof(double *));
    double dm[N - 2] = {0};
    for (int i = 0; i < N; i++) {
        u[i] = (double *) malloc(N * sizeof(double));
        f[i] = (double *) malloc(N * sizeof(double));
//        dm[i] = 0.0;
    }

    init(u);
    init_f(f);


    for (int n = 0; n < N; n++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f\t", u[n][j]);
        }
        printf("\n");
    }

    double h = 1.0 / (N + 1);
    int iter = 0;
    double temp;
    double dmax;

    omp_lock_t dmax_lock;
    omp_init_lock(&dmax_lock);
    int nx;
    int i;
    int j;
    double d;
    do {
        j = 0;
        dmax = 0.0;
        iter++;
        for (nx = 1; nx <= N - 2; nx++) {
            dm[nx] = 0;
#pragma omp parallel for shared(u, nx, dm)\
                            private(i, j, temp, d)
            for (i = 1; i < nx - 2; i++) {
                j = nx + 1 - i;
                temp = u[i][j];
//                printf("%.2f\t", temp);
                u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1] - h * h * f[i][j]);
                d = fabs(temp - u[i][j]);
                if (dm[i] < d) dm[i] = d;
//                if(dmax < dm) dmax = dm;
            }
//            omp_set_lock(&dmax_lock);
//            if ( dmax < dm ) dmax = dm;
//            omp_unset_lock(&dmax_lock);
        }
        for (nx = (N - 3); nx > 0; nx--) {
#pragma omp parallel for shared(u, nx, dm)\
                            private(i, j, temp, d)
            for (i = N - nx + 1; i <= N - 2; i++) {
                j = 2 * (N - 1) - nx - i + 1;
                temp = u[i][j];
                u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1] - h * h * f[i][j]);
                d = fabs(temp - u[i][j]);
                if (dm[i] < d) dm[i] = d;
            }
        }
//
#pragma omp parallel for shared(dm, dmax) \
private(i)
        for (i = 1; i <= N - 2; i++) {
            omp_set_lock(&dmax_lock);
            if (dmax < dm[i]) dmax = dm[i];
            omp_unset_lock(&dmax_lock);
        }
    } while (dmax > EPSILON && iter <= MAX_ITER);
    printf("\n");


    printf("Converged after %d iterations with residual: %f \n", iter, dmax);

    for (int n = 0; n < N; n++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f\t \t", u[n][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++) {
        free(u[i]);
        free(f[i]);
    }
    free(u);
    free(f);
    return 0;
}
