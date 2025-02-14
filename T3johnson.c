#include <stdio.h>
#include <stdlib.h>

#define INF 268435455
#define MAXN 605
#define MAXM 42005
#define MAX_HEAP_SIZE 1000

typedef struct {
    int vertex;
    int dist;
} Node;

typedef struct {
    Node nodes[MAX_HEAP_SIZE];
    int size;
} MinHeap;

void init_heap(MinHeap* heap) {
    heap->size = 0;
}

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int index) {
    int parent_index = (index - 1) / 2;
    if (parent_index >= 0 && heap->nodes[parent_index].dist > heap->nodes[index].dist) {
        swap(&heap->nodes[parent_index], &heap->nodes[index]);
        heapify_up(heap, parent_index);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int left_child_index = 2 * index + 1;
    int right_child_index = 2 * index + 2;
    int smallest = index;

    if (left_child_index < heap->size && heap->nodes[left_child_index].dist < heap->nodes[smallest].dist) {
        smallest = left_child_index;
    }

    if (right_child_index < heap->size && heap->nodes[right_child_index].dist < heap->nodes[smallest].dist) {
        smallest = right_child_index;
    }

    if (smallest != index) {
        swap(&heap->nodes[smallest], &heap->nodes[index]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int vertex, int dist) {
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].dist = dist;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

Node heap_pop(MinHeap* heap) {
    Node root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

void decrease_key(MinHeap* heap, int vertex, int dist) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->nodes[i].vertex == vertex) {
            heap->nodes[i].dist = dist;
            heapify_up(heap, i);
            return;
        }
    }
}

struct Edge {
    int u, v, w;
};

struct Graph {
    struct Edge edges[MAXM];
    int num_edges;
};

void add_edge(struct Graph* G, int index, int u, int v, int w) {
    G->edges[index].u = u;
    G->edges[index].v = v;
    G->edges[index].w = w;
    //printf("Edge added: %d --(%d)--> %d\n", u, w, v);
}

void add_source_vertex(struct Graph* G, int n) {
    G->num_edges += n;
    for (int i = 1; i <= n; i++) {
        G->edges[G->num_edges - n + i - 1].u = 0;
        G->edges[G->num_edges - n + i - 1].v = i;
        G->edges[G->num_edges - n + i - 1].w = 0;
        //printf("Source edge added: 0 --(0)--> %d\n", i);
    }
}

int bellman_ford(struct Graph* G, int source, int n, int dist[]) {
    int u, v, w;

    for (int i = 0; i <= n; i++) {
        dist[i] = INF;
    }

    dist[source] = 0;

    for (int i = 0; i < n; i++) {
        //printf("Iteration %d\n", i+1);
        for (int j = 0; j < G->num_edges; j++) {
            u = G->edges[j].u;
            v = G->edges[j].v;
            w = G->edges[j].w;

            //printf("Checking edge %d --(%d)--> %d\n", u, w, v);
            if (dist[u] != INF && dist[v] > dist[u] + w) {
                //printf("Updating distance to vertex %d\n", v);
                dist[v] = dist[u] + w;
            }
        }
        for (int k = 0; k < n; k++) {
            //printf("dist[%d]: %d\n", k, dist[k]);
        }
    }

    //printf("Checking for negative-weight cycles\n");
    for (int j = 0; j < G->num_edges; j++) {
        u = G->edges[j].u;
        v = G->edges[j].v;
        w = G->edges[j].w;

        //printf("Checking edge %d --(%d)--> %d\n", u, w, v);
        if (dist[u] != INF && dist[v] > dist[u] + w) {
            //printf("Negative-weight cycle detected\n");
            return 0;  
        }
    }

    //printf("No negative-weight cycles detected\n");
    return 1; 
}


void generate_adj_matrix(struct Graph* G, int h[], int n, int** adj_matrix) {
    int u, v, w;
    
    //printf("Generating adjacency matrix...\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                adj_matrix[i][j] = 0;
            } else {
                adj_matrix[i][j] = INF;
            }
        }
    }
    //printf("initialization completed\n");
    
    for (int i = 0; i < G->num_edges; i++) {
        u = G->edges[i].u;
        v = G->edges[i].v;
        w = G->edges[i].w;
        
        if (u == 0 || v == 0) 
            continue; 
        
        adj_matrix[u - 1][v - 1] = w + h[u] - h[v];
        //printf("Edge: %d -> %d, Weight: %d, Adjusted Weight: %d\n", u, v, w, adj_matrix[u - 1][v - 1]);
    }
}


void dijkstra(int source, int n, int** adj_matrix, int dist[]) {
    int i, j;
    int visited[MAXN] = {0};
    MinHeap heap;

    init_heap(&heap);

    for (i = 0; i < n; i++) {
        dist[i] = INF;
    }

    dist[source] = 0;
    heap_push(&heap, source, 0);

    while (heap.size > 0) {
        Node node = heap_pop(&heap);
        int u = node.vertex;
        //printf("Vertex popped from heap: %d\n", u);

        if (!visited[u]) {
            visited[u] = 1;

            for (j = 0; j < n; j++) {
                if (adj_matrix[u][j] != INF && !visited[j]) {
                    int new_dist = dist[u] + adj_matrix[u][j];

                    if (new_dist < dist[j]) {
                        dist[j] = new_dist;
                        heap_push(&heap, j, new_dist);
                        //printf("Updated distance to vertex %d: %d\n", j, new_dist);
                    }
                }
            }
        }
    }
}

void restore(int n, int **all_pairs_dist, int dist[]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (all_pairs_dist[i][j] != INF) {
                all_pairs_dist[i][j] = all_pairs_dist[i][j] - dist[i + 1] + dist[j + 1];
            }
        }
    }
}

void johnson(struct Graph* G, int n, int** all_pairs_dist) {
    //printf("begin\n");
    int dist[MAXN];  
    int** adj_matrix = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        adj_matrix[i] = (int*)malloc(sizeof(int) * n);
    }

    
    int tmp_dist[MAXN]; 
        
    //printf("begin bellman\n");
    if (!bellman_ford(G, 0, n + 1, dist)) {
        printf("negative-weight cycle\n");
        exit(0);
    }
    //printf("bellman complete\n");
    generate_adj_matrix(G, dist, n, adj_matrix);
    //printf("generate_adj_matrix complete\n");

    for (int i = 0; i < n; i++) {
        dijkstra(i, n, adj_matrix, tmp_dist);

        for (int j = 0; j < n; j++) {
            if (tmp_dist[j] != INF) {
                all_pairs_dist[i][j] = tmp_dist[j];
            } else {
                all_pairs_dist[i][j] = INF;
            }
            //printf("Shortest distance from %d to %d: %d\n", i, j, all_pairs_dist[i][j]);
        }
    }

    restore(n, all_pairs_dist, dist);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //printf("Shortest distance from %d to %d: %d\n", i, j, all_pairs_dist[i][j]);
        }
    }
    for (int i = 0; i < n; i++) {
        free(adj_matrix[i]);
    }
    free(adj_matrix);

}

int main() {
    int n, m, q, u, v, w;
    struct Graph G;
    //freopen("D:/oj/fourth_oj/in.txt", "r", stdin);

    scanf("%d %d", &n, &m);
    G.num_edges = m;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        add_edge(&G, i, u, v, w);
    }
    int **all_pairs_dist = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        all_pairs_dist[i] = (int*)malloc(n * sizeof(int));
    }
    add_source_vertex(&G, n);  
    johnson(&G, n, all_pairs_dist);

    scanf("%d", &q);
    //printf("q=%d\n",q);
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &u, &v);
        printf("%d\n", all_pairs_dist[u - 1][v - 1]);
    }

    for (int i = 0; i < n; i++) {
        free(all_pairs_dist[i]);
    }
    free(all_pairs_dist);
    return 0;
}