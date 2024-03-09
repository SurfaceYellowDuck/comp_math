#include <stdio.h>

void iterate_matrix_in_squares(int matrix[6][6], int rows, int cols, int square_size) {
    for (int i = 1; i < rows; i += square_size) {
        for (int j = 1; j < cols; j += square_size) {
            printf("Square:\n");
            for (int x = i; x < (i + square_size) && x < rows + 1; x++) {
                for (int y = j; y < (j + square_size) && y < cols + 1; y++) {
                    printf("%d ", matrix[x][y]);
                }
                printf("\n");
            }
        }
    }
}

int main() {
    int matrix[6][6] = {
            { 3,5, 1, 2, 3, 4},
            {4, 6, 5, 6, 7, 8},
            {5, 7, 9, 10, 11, 12},
            {6, 8,13, 14, 15, 16},
            {7, 13, 8, 14, 15, 16},
            {8, 6, 5, 6, 7, 8},

    };
    int rows = 6;
    int cols = 6;
    int square_size = 2;

    iterate_matrix_in_squares(matrix, rows, cols, square_size);

    return 0;
}