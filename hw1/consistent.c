//#include "/usr/local/opt/libomp/include/omp.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>

//#define N 10      // Размер сетки
#define MAX_ITER 1000 // Максимальное количество итераций
#define EPSILON 0.1

void init(double **u, int N) {
    double h = 1.0 / (N - 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
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
                u[i][j] = 0; // Внутри области D задаем случайное значение
            }
        }
    }
}

void init_f(double **f, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            f[i][j] = 0.0;
        }
    }
}
//grid_size[l]
int main() {
//    int grid_size[] = {66, 82, 162,70, 87, 172};
//    int grid_size[] = {258, 642, 1282,256, 637, 1272};
    int NB = 512;
    int NB_bad = 511;
    unsigned int grid_size[] = {NB * 2, NB * 5, NB * 10, NB * 16, NB_bad * 2 + 2, NB_bad * 5 + 2, NB_bad * 10 + 2, NB_bad * 16 + 2};
    for (int l = 0; l < (sizeof(grid_size) / sizeof(int)); l++) {

        clock_t start_time, end_time;
        double time_taken;
        start_time = clock(); // Засекаем начальное время

        double **u = (double **) malloc(grid_size[l] * sizeof(double *));
        double **f = (double **) malloc(grid_size[l] * sizeof(double *));

        for (int i = 0; i < grid_size[l]; i++) {
            u[i] = (double *) malloc(grid_size[l] * sizeof(double));
            f[i] = (double *) malloc(grid_size[l] * sizeof(double));
        }

        init(u, grid_size[l]);
        init_f(f, grid_size[l]);
//        for (int n = 0; n < grid_size[l]; n++) {
//            for (int j = 0; j < grid_size[l]; j++) {
//                printf("%.2f\t", u[n][j]);
//            }
//            printf("\n");
//        }

        double dmax = 0.0;
        double h = 1.0 / (grid_size[l] + 1);
        int iter = 0;
        double temp;
        double dm = dmax;

        do {
            dmax = 0.0;
            iter++;
            for (int i = 1; i < grid_size[l] - 1; i++) {
                for (int j = 1; j < grid_size[l] - 1; j++) {
                    temp = u[i][j];
//                printf("%.2f\t", temp);
                    u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1] - h * h * f[i][j]);
                    dm = fabs(temp - u[i][j]);
                    if (dmax < dm) dmax = dm;
                }
            }
        } while (dmax > EPSILON && iter <= MAX_ITER);


        printf("Converged after %d iterations with residual: %f \n", iter, dmax);
        end_time = clock(); // Засекаем конечное время
        time_taken = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // Вычисляем время
        printf("Program execution time: %f seconds for %d grid\n\n", time_taken, grid_size[l]);
//        for (int n = 0; n < grid_size[l]; n++) {
//            for (int j = 0; j < grid_size[l]; j++) {
//                printf("%.2f\t \t", u[n][j]);
//            }
//            printf("\n");
//        }

        for (int i = 0; i < grid_size[l]; i++) {
            free(u[i]);
            free(f[i]);
        }
        free(u);
        free(f);

    }
    return 0;
}
