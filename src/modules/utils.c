#include "utils.h"

void multiplyMatrices4x4(float result[4][4], float a[4][4], float b[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int minInt(int a, int b) {
    return a > b ? b : a;
}

int maxInt(int a, int b) {
    return a < b ? b : a;
}

int factorial(int n) {
    return n > 0 ? n * factorial(n - 1) : 1;
}

int comb(int n, int i) {
    return factorial(n) / (factorial(i) * factorial(n - i));
}
