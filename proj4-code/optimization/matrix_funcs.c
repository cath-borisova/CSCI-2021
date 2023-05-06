#include <stdlib.h>
#include "matrix.h"

void get_every_fifth(const matrix_t *matrix, long results[RESULTS_LEN]) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            int q = j % RESULTS_LEN;
            results[q] += MGET(matrix, i, j);
        }
    }
}

long get_every(const matrix_t *matrix) {
    long sum = 0;
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            sum += MGET(matrix, i, j);
        }
    }
    return sum;
}

void initialize_results(long results[RESULTS_LEN]) {
    for (int i = 0; i < RESULTS_LEN; i++) {
        results[i] = 0;
    }
}

void set_up_matrix(matrix_t *matrix, int rows, int cols) {
    if (matrix == NULL) {
        return;
    }
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(sizeof(int) * rows * cols);
    srand(2021);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MSET(matrix, i, j, rand() % 100);
        }
    }
}

void free_matrix(matrix_t *matrix) {
    free(matrix->data);
}
