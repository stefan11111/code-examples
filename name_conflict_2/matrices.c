#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_vector {
    int size;
    int size_1;
    int size_2;
    int **matrices;
} matrix_vec_t;

int* matrix_multiply (const int * restrict A, int A_size_1, int A_size_2, const int * restrict B, int B_size_1, int B_size_2)
{
    int *result = malloc(A_size_1 * B_size_2 * sizeof(int));
    if (!result) {
        return 0;
    }
#if 0
    int size = A_size_2 < B_size_1 ? A_size_2 : B_size_1;
#else
    int size = A_size_2;
#endif
    int i, j, k;
    for(i = 0; i < A_size_1; i++) {
        for(j = 0; j < B_size_2; j++) {
            int sum = 0;
            for(k = 0; k < size; k++) {
                sum += A[A_size_2 * i + k] * B[k * B_size_2 + j];
            }
            result[A_size_2 * i + j] = sum;
        }
    }
    return result;
}

matrix_vec_t vector_multiply (matrix_vec_t A, matrix_vec_t B)
{
#if 0
    int size = A.size < B.size ? A.size : B.size;
#else
    int size = A.size;
#endif
    matrix_vec_t result = (matrix_vec_t){.size = size, .size_1 = A.size_1, .size_2 = B.size_2, .matrices = malloc(size * sizeof(int*))};
    for (int i = 0; i < size; i++) {
        result.matrices[i] = matrix_multiply (A.matrices[i], A.size_1, A.size_2, B.matrices[i], B.size_1, B.size_2);
    }
    return result;
}

int main()
{
    int x, n1, m1, n2, m2;
    scanf("%d %d %d %d %d", &x, &n1, &m1, &n2, &m2);
    matrix_vec_t vec1 = (matrix_vec_t){.size = x, .size_1 = n1, .size_2 = m1, .matrices = malloc(x * sizeof(int*))};
    if (!vec1.matrices) {
        return 0;
    }
    matrix_vec_t vec2 = (matrix_vec_t){.size = x, .size_1 = n2, .size_2 = m2, .matrices = malloc(x * sizeof(int*))};
    if (!vec2.matrices) {
        return 0;
    }

    for (int i = 0; i < x; i++) {
        vec1.matrices[i] = malloc(n1 * m1 * sizeof(int));
        if (!vec1.matrices[i]) {
            return 0;
        }
        vec2.matrices[i] = malloc(n2 * m2 * sizeof(int));
        if (!vec2.matrices[i]) {
            return 0;
        }
    }

    for (int k = 0; k < x; k++) {
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                 scanf("%d", &vec1.matrices[k][m1 * i + j]);
            }
        }
    }

    for (int k = 0; k < x; k++) {
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                 scanf("%d", &vec2.matrices[k][m2 * i + j]);
            }
        }
    }

    matrix_vec_t vec = vector_multiply (vec1, vec2);

    char *buf = malloc(n1 + sizeof("\n\n"));
    if (buf) {
        *buf = '\n';
        memset(buf + 1, '-', n1);
        *(buf + 1 + n1) = '\n';
        *(buf + 1 + n1 + 1) = '\0';
    }
    else {
        buf = "\n\n\n";
    }

    for (int k = 0; k < x; k++) {
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m2; j++) {
                 printf("%d ", vec.matrices[k][m2 * i + j]);
            }
            printf("\n");
        }
        printf("%s", buf);
    }
    return 0;
}
