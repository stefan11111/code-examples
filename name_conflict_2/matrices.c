#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_vector {
    int size;
    int size_1;
    int size_2;
    int **matrices;
} matrix_vec_t;

int* matrix_multiply (const int *A, int A_size_1, int A_size_2, const int *B, int B_size_1, int B_size_2)
{
    int *result = malloc(A_size_1 * B_size_2 * sizeof(int));
    if (!result) {
        printf("Out of memory\n");
        return 0;
    }

    if (A_size_2 < B_size_1) {
        return 0;
    }

    int size = A_size_2;
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
#if 1
    int size = A.size < B.size ? A.size : B.size;
#else
    int size = A.size;
#endif
    matrix_vec_t result = (matrix_vec_t){.size = size, .size_1 = A.size_1, .size_2 = B.size_2, .matrices = malloc(size * sizeof(int*))};
    if (!result.matrices) {
        printf("Out of memory\n");
        return result;
    }
    for (int i = 0; i < size; i++) {
        result.matrices[i] = matrix_multiply (A.matrices[i], A.size_1, A.size_2, B.matrices[i], B.size_1, B.size_2);
    }
    return result;
}

int* log_power (const int *A, int size, int power)
{
    if (power < 0) {
        printf("negative power not supported\n");
        return 0;
    }

    int *result = malloc(size * size * sizeof(int));

    if (!result) {
        printf("Out of memory\n");
        return 0;
    }

    if (power == 0) {
        memset(result, 0, size * size * sizeof(int));
        int i;
        for (i = 0; i < size; i++) {
            result[size * i + i] = 1;
        }
        return result;
    }

    if (power == 1) {
        memcpy(result, A, size * size * sizeof(int));
        return result;
    }

    if (power % 2) {
        int *tmp_1 = log_power(A, size, (power - 1) / 2);
        int *tmp_2 = matrix_multiply(tmp_1, size, size, tmp_1, size, size);
        result = matrix_multiply(A, size, size, tmp_2, size, size);
        free(tmp_1);
        free(tmp_2);
        return result;
    }

    int *tmp = log_power(A, size, power / 2);
    result = matrix_multiply(tmp, size, size, tmp, size, size);
    free(tmp);
    return result;
}

int main()
{
    FILE *f = stdin;
    char filename[100];
    printf("Read from file? [y/n] (default n)\n");
    scanf("%c", filename);
    if (*filename == 'y' || *filename == 'Y') {
        printf("Enter input filename (99 char's max):\n");
        scanf("%99s", filename);
        f = fopen(filename, "r");
        if (!f) {
            f = stdin;
        }
    }
    int x, n1, m1, n2, m2;

    char op = 0;

    if (f == stdin)
    printf("Which matrix operation to perform? [1 for vector multiplication, 2 for raising to a power]\n");
    fscanf(f, "%hhd", &op);
    if (op == 2) {
        int size, power;
        if (f == stdin)
        printf("Enter size of the square matrix and power to raise it to\n");
        fscanf(f, "%d %d", &size, &power);
        if (power < 0) {
            printf("Power must be a natural number");
            return 0;
        }
        int *matrix = malloc(size * size * sizeof(int));
        if (!matrix) {
            printf("Out of memory\n");
            return 0;
        }

        if (f == stdin)
        printf("Enter elements of the matrix\n");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                 fscanf(f, "%d", &matrix[size * i + j]);
            }
        }

        int *result = log_power(matrix, size, power);

        printf("Output matrix:\n\n");

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                 printf("%d ", result[size * i + j]);
            }
            printf("\n");
        }

        return 0;
    }


    if (f == stdin)
    printf("Enter, in order:\n"
           "number of matrices\n"
           "number of lines of the first matrix vector\n"
           "number of columns of the first matrix vector\n"
           "number of lines of the second matrix vector\n"
           "number of columns of the second matrix vector\n\n");
    fscanf(f, "%d %d %d %d %d", &x, &n1, &m1, &n2, &m2);
    if (m1 != n2) {
        printf("Number of columns of the first matrix must match the number of columns of the second matrix\n");
        return 0;
    }
    matrix_vec_t vec1 = (matrix_vec_t){.size = x, .size_1 = n1, .size_2 = m1, .matrices = malloc(x * sizeof(int*))};
    if (!vec1.matrices) {
        printf("Out of memory\n");
        return 0;
    }
    matrix_vec_t vec2 = (matrix_vec_t){.size = x, .size_1 = n2, .size_2 = m2, .matrices = malloc(x * sizeof(int*))};
    if (!vec2.matrices) {
        printf("Out of memory\n");
        return 0;
    }

    for (int i = 0; i < x; i++) {
        vec1.matrices[i] = malloc(n1 * m1 * sizeof(int));
        if (!vec1.matrices[i]) {
            printf("Out of memory\n");
            return 0;
        }
        vec2.matrices[i] = malloc(n2 * m2 * sizeof(int));
        if (!vec2.matrices[i]) {
            printf("Out of memory\n");
            return 0;
        }
    }

    if (f == stdin)
    printf("Enter elements of the first set of matrices\n\n");

    for (int k = 0; k < x; k++) {
        if (f == stdin)
        printf("Enter elements of the %d'th matrix\n", k + 1);
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                 fscanf(f, "%d", &vec1.matrices[k][m1 * i + j]);
            }
        }
    }

    if (f == stdin)
    printf("Enter elements of the second set of matrices\n\n");

    for (int k = 0; k < x; k++) {
        if (f == stdin)
        printf("Enter elements of the %d'th matrix\n", k + 1);
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                 fscanf(f, "%d", &vec2.matrices[k][m2 * i + j]);
            }
        }
    }

    matrix_vec_t vec = vector_multiply (vec1, vec2);

    if (!vec.matrices) {
        printf("vector_multiply failed\n");
        return 0;
    }

    char *buf = malloc(n1 + sizeof("\n\n")); /* freed by return from main() */
    if (buf) {
        *buf = '\n';
        memset(buf + 1, '-', n1);
        *(buf + 1 + n1) = '\n';
        *(buf + 1 + n1 + 1) = '\0';
    }
    else {
        buf = "\n\n\n";
    }

    printf("%s", buf);

    for (int k = 0; k < x; k++) {
        if (!vec.matrices[k]) {
            printf("matrix_multiply failed\n");
            return 0;
        }
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
