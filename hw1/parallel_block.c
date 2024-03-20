#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <omp.h>
#include <time.h>


//#define N 10 //количество строк и столбцов матрицы
//#define NB 2
#define eps 0.1

double u_(double x, double y, double h) {
    if (y == 0){
        return 100.0 - 200.0 * pow(x, 1) * h;
    }
    if(x == 0){
        return 100 - 200 * pow(y, 1) * h;
    }
    if(y == 1){
        return -100 + 200 * pow(x, 1) * h;
    }
    if(x == 1) {
        return -100 + 200 * pow(y, 1) * h;
    }
    return 0;
}
void init(double **u, int N){
    double h = 1.0 / (N - 1);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                if (i == 0) {
                    u[i][j] = 20 * u_(i, j, h); // Граничное условие f(0, y) = 100 - 200y
                    if (j == N - 1) {
                        continue;
                    }
                }

                if (i == (N - 1)) {
                    u[i][j] = 20 * u_(i, j, h); // Граничное условие f(1, y) = -100 + 200y
                }

                if (j == 0) {
                    u[i][j] = 20 * u_(i, j, h); // Граничное условие f(x, 0) = 100 - 200x
                }

                if (j == (N - 1)) {
                    u[i][j] = 20 * u_(i, j, h); // Граничное условие f(x, 1) = -100 + 200x
                }
            } else {
                int num = (rand() %
                           (10 - (-10) + 1)) + (-10);
                u[i][j] = 20 * num; // Внутри области D задаем случайное значение
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

int min(int a, int b) {
    return a < b ? a : b;
}

double calc_block(double **u, double **f, int i, int j, double h, int N, int NB) {
    int x0 = 1 + i * NB;
    int xmax = min(x0 + NB, N - 1);
    int y0 = 1 + j * NB;
    int ymax = min(y0 + NB, N - 1);
    double dm_block = 0;
//                printf("Square:\n");
    for (int x = x0; x < xmax; x++) {
        for (int y = y0; y < ymax; y++) {
            double temp = u[x][y];
            u[x][y] = 0.25 * (u[x - 1][y] + u[x + 1][y] + u[x][y - 1] + u[x][y + 1] -
                              h * h * f[x][y]);
//            printf("%f ", u[x][y]);
            double d = fabs(temp - u[x][y]);
            if (dm_block < d){
                dm_block = d;
            }
        }
    }
//    printf("\n");
    return dm_block;
}

void async_alg5(double **u, double **f, int N, int NB) {
    double dmax;
    unsigned int iter = 0;
    int work_matrix_size = N - 2;
    int numb_of_blocks = work_matrix_size / NB;
    if (NB * numb_of_blocks != work_matrix_size)
        numb_of_blocks += 1;
    double h = 1.0 / (N - 1);
    double* dm = calloc(numb_of_blocks, sizeof(*dm));
    omp_lock_t dmax_lock;
    omp_init_lock(&dmax_lock);
    do {
        iter ++;
        dmax = 0.0;
        for (int nx = 0; nx < numb_of_blocks; nx++) {
            dm[nx] = 0;
            double d;
            int i, j;
#pragma omp parallel for shared(nx, dm) private(i, j, d)
            for (i = 0; i < nx + 1; i++) {
                j = nx - i;
                d = calc_block(u, f, i, j, h, N, NB);
                if (dm[i] < d) {
                    dm[i] = d;
                }
//                    printf("\n");
            }
        }
        // затухание волны
        for (int nx = numb_of_blocks - 2; nx >= 0; nx--) {
            int i, j;
            double d;
#pragma omp parallel for shared(nx, dm) private(i, j, d)
            for (i = numb_of_blocks - nx - 1; i < numb_of_blocks; i++) {
                j = numb_of_blocks + ((numb_of_blocks - 2) - nx) - i;
                d = calc_block(u, f, i, j, h, N, NB);
                if (dm[i] < d) {
                    dm[i] = d;
                }
            }
        }
        int i;
//#pragma omp parallel for shared(dm, dmax) \
//private(i)
        for (i = 1; i <= numb_of_blocks; i++) {
//            omp_set_lock(&dmax_lock);
            if (dmax < dm[i]) dmax = dm[i];
//            omp_unset_lock(&dmax_lock);
        }
//        printf("%f \n",dmax);
    } while (dmax > eps);
    printf("Converged after %d iterations with residual: %f \n", iter, dmax);
    free(dm);
}

int main() {
    omp_set_num_threads(8);
//    int NB = 4096;
//    int NB_bad = 4097;
    int NB = 125;
//    int NB_bad = 125;
    unsigned int matrix_size[] = {1002};
//    unsigned int matrix_size[] = {NB * 4, NB * 8};

//    unsigned int matrix_size[] = {NB_bad * 8, NB_bad * 24};


//    int matrix_size[] = {258, 642, 1282,256, 637, 1272};
//    int NB = 16;
//        int NB = 128;
    for (int l = 0; l < (sizeof(matrix_size) / sizeof(int)); l++) {
        double start_time, end_time;
        double time_taken;
        start_time = omp_get_wtime(); // Засекаем начальное время

        double **u = (double **) malloc(matrix_size[l] * sizeof(double *));
        double **f = (double **) malloc(matrix_size[l] * sizeof(double *));
        for (int i = 0; i < matrix_size[l]; i++) {
            u[i] = (double *) malloc(matrix_size[l] * sizeof(double));
            f[i] = (double *) malloc(matrix_size[l] * sizeof(double));
        }
        init(u, matrix_size[l]);
        init_f(f, matrix_size[l]);

//    for (int n = 0; n < N; n++) {
//        for (int j = 0; j < N; j++) {
//            printf("%.2f\t", u[n][j]);
//        }
//        printf("\n");
//    }
        async_alg5(u, f, matrix_size[l], NB);
//        printf("Converged after %d iterations with residual: %f \n", iter, dmax);
        end_time = omp_get_wtime(); // Засекаем конечное время
        time_taken = (end_time - start_time) ; // Вычисляем время
        printf("Program execution time: %f seconds for %d grid\n\n", time_taken, matrix_size[l]);
//    printf("\n");

    for (int n = 0; n < matrix_size[0]; n++) {
        for (int j = 0; j < matrix_size[0]; j++) {
            printf("%.2f\t", u[n][j]);
        }
        printf("\n");
    }
//    }
        for (int i = 0; i < matrix_size[l]; i++) {
            free(u[i]);
            free(f[i]);
        }
        free(u);
        free(f);
    }


}
