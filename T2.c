#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int u, v, w;
};

int parent[5005];

void init(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
}

int findRoot(int x) {
    if (x == parent[x]) {
        return x;
    } else {
        return parent[x] = findRoot(parent[x]);
    }
}

void unionSets(int x, int y) {
    int rootX = findRoot(x);
    int rootY = findRoot(y);
    if (rootX != rootY) {
        parent[rootX] = rootY;
    }
}

void quickSort(struct Edge* edges, int low, int high) {
    if (low < high) {
        struct Edge pivot = edges[low];
        int i = low + 1;
        int j = high;

        while (i <= j) {
            while (i <= j && edges[i].w < pivot.w) {
                i++;
            }
            while (i <= j && edges[j].w > pivot.w) {
                j--;
            }
            if (i <= j) {
                struct Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
                i++;
                j--;
            }
        }

        struct Edge temp = edges[low];
        edges[low] = edges[j];
        edges[j] = temp;

        quickSort(edges, low, j - 1);
        quickSort(edges, j + 1, high);
    }
}

int kruskal(struct Edge* edges, int n, int m) {
    int sum = 0;
    init(n);

    for (int i = 0; i < m; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        if (findRoot(u) != findRoot(v)) {
            sum += w;
            unionSets(u, v);
        }
    }

    return sum;
}

int secondMinimumSpanningTree(struct Edge* edges, int n, int m, int minWeight) {
    int secondMinWeight = -1;

    for (int i = 0; i < m; i++) {
        if (edges[i].w == minWeight) {
            continue;
        }

        init(n);
        int sum = 0;
        int count = 0;

        for (int j = 0; j < m; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].w;

            if (findRoot(u) != findRoot(v)) {
                sum += w;
                count++;
                unionSets(u, v);
            }
        }

        if (count == n - 1) {
            if (secondMinWeight == -1 || sum < secondMinWeight) {
                secondMinWeight = sum;
            }
        }
    }

    return secondMinWeight;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);

    struct Edge* edges = (struct Edge*)malloc(m * sizeof(struct Edge));
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d", &edges[i].u, &edges[i].v, &edges[i].w);
    }

    quickSort(edges, 0, m - 1);

    int minWeight = kruskal(edges, n, m);
    printf("%d\n", minWeight);

    int secondMinWeight = secondMinimumSpanningTree(edges, n, m, minWeight);
    printf("%d\n", secondMinWeight);

    free(edges);

    return 0;
}
