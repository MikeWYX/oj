#include<stdio.h>
#include<stdlib.h>

#define INF 268435455

int main() {
    int n, m, i, j, k, u, v, w, q;

    scanf("%d %d", &n, &m);

    int **d = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        d[i] = (int*)malloc(n * sizeof(int));
        for (j = 0; j < n; j++) {
            if (i == j) {
                d[i][j] = 0;
            }
            else 
                d[i][j] = INF;
        }
    }

    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        d[u-1][v-1] = w;
    }

    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (d[i][k] != INF && d[k][j] != INF && d[i][j] > d[i][k] + d[k][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        if (d[i][i] < 0) {
            printf("negative-weight cycle\n");
            return 0;
        }
    }

    scanf("%d", &q);
    for (i = 0; i < q; i++) {
        scanf("%d %d", &u, &v);
        if (d[u-1][v-1] == INF)
            printf("%d\n", INF);
        else
            printf("%d\n", d[u-1][v-1]);
    }

    for (i = 0; i < n; i++) {
        free(d[i]);
    }
    free(d);

    return 0;
}
