#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-9

void transform1(double** matrix, int i, int j, double* det, int matSize) {
    for(int k = 0; k < matSize; k++) {
        double temp = matrix[i][k];
        matrix[i][k] = matrix[j][k];
        matrix[j][k] = temp;
    }

    *det = -*det;
}

void transform2(double** matrix, int i, int j, double factor, int matSize) {
    for(int k = 0; k < matSize; k ++) {
        matrix[i][k] += factor * matrix[j][k];
    }
}

double** readMatrix(int matSize) {
    double** matrix = malloc(matSize*sizeof(double*));
    
    if(!matrix) return NULL;
    
    for(int i = 0; i < matSize; i++) {
        matrix[i] = malloc(matSize*sizeof(double));
        if(!matrix[i]) {
            for(int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    
    for(int i = 0; i < matSize; i++) {
        for(int j = 0; j < matSize; j++) {
            if(scanf("%lf", &matrix[i][j]) != 1) {
                for(int i = 0; i < matSize; i++) {
                    free(matrix[i]);
                }

                free(matrix);
                return NULL;
            }
        }
    }

    return matrix;
}

void triangulate(double** matrix, int matSize, double* det) {
    for(int i = 0; i < matSize; i++) {
        int maxRow = i;

        for(int k = i + 1; k < matSize; k++) {
            if(fabs(matrix[k][i]) > fabs(matrix[maxRow][i])) {
                maxRow = k;
            }
        }

        if(fabs(matrix[maxRow][i]) < EPS) {
            *det = 0;
            return;
        }

        if(maxRow != i) {
            transform1(matrix, i, maxRow, det, matSize);
        }

        for(int j = i + 1; j < matSize; j++) {
            double div = -(matrix[j][i] / matrix[i][i]);
            transform2(matrix, j, i, div, matSize);
        }

        *det *= matrix[i][i];
    }
}

void freeMatrix(double** matrix, int matSize) {
    if(matrix) {
        for(int i = 0; i < matSize; i++) {
        free(matrix[i]);
        }
        
        free(matrix);
    }
}

int main() {
    int mSize = 0;
    double det = 1.0;
    if(scanf("%d", &mSize) != 1) return -1;

    double** mat = readMatrix(mSize);

    triangulate(mat, mSize, &det);
    printf("det = %lf\n", det);

    freeMatrix(mat, mSize);
    return 0;
}