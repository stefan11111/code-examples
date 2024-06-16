#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int x;
    int y;
    int z;
} nestemata_t;

nestemata_t nestemate[100001];

int t, n, start, final;

static inline bool disjuncte(nestemata_t *a, nestemata_t *b)
{
    for(int *i = (int*)a; i < ((int*)a) + 3; i++) {
        for(int *j = (int*)b; j < ((int*)a) + 3; j++) {
            if(*i == *j) {
                return 0;
            }
        }
    }
    return 1;
}

int main()
{
    FILE *f = fopen("nestemate.in", "r");
    FILE *g = fopen("nestemate.out", "w");

    fscanf(f, "%d", &t);

    for(int it = 1; it <= t; it++) {
        fscanf(f, "%d", &n);
        fscanf(f, "%d %d", &start, &final);

        for(int i = 1; i <= n; i++) {
            fscanf(f, "%d %d %d", &nestemate[i].x, &nestemate[i].y, &nestemate[i].z);
        }

        bool A[n + 1][n + 1];
        memset(A, 0, sizeof(A));

        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if(!disjuncte(nestemate + i, nestemate + j) && i != j) {
                    A[i][j] = 1;
                }
            }
        }

        int st = 1, fin = 1;

        int nr_nivele = 0;
        int nr_nivele_min = -2;

        int coada[n + 1];
        int viz[n + 1];

        memset(coada, 0, sizeof(coada));
        memset(viz, 0, sizeof(viz));

        coada[st] = start;
        viz[start] = 1;

        int fin_vechi = fin;

        while(st <= fin) {
            for(int i = 1; i <= n; i++) {
                if(A[st][i] == 1 && viz[i] == 0) {
                    coada[++fin] = i;
                    viz[i] = 1;
                    if(i == final && nr_nivele_min == -2) {
                        nr_nivele_min = nr_nivele;
                    }
                }
            }
            if(st > fin_vechi) {
                nr_nivele++;
                fin_vechi = fin;
            }
            st++;
        }

        fprintf(g, "%d\n", nr_nivele_min + 1);
    }
    return 0;
}
