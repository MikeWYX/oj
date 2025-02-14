#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int parent;
    int sign;
    int child_count;
    int* children;
}Node;

int find_parent(Node* nodes, int id, int r){
    while(r > 0 && id != 1){
        id = nodes[id].parent;
        r--;
    }
    return id;
}

void cover(Node* nodes, int id, int r){
    if(r == 0){
        nodes[id].sign = 1;
        return;
    }
    nodes[id].sign = 1;
    for(int i = 0; i < nodes[id].child_count; i++){
        cover(nodes, nodes[id].children[i], r-1);
    }
    cover(nodes, nodes[id].parent, r-1);
}

int main(){
    int n, r, id, min = 0;
    scanf("%d %d", &n, &r);
    Node* nodes = (Node*)malloc((n+1) * sizeof(Node));  
    for(int i = 1; i <= n; i++){
        nodes[i].sign = 0;
        nodes[i].child_count = 0;
        nodes[i].parent = 1;
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &(nodes[i].child_count));  
        nodes[i].children = (int*)malloc(nodes[i].child_count * sizeof(int));  
        for (int j = 0; j < nodes[i].child_count; j++) {
            scanf("%d", &(nodes[i].children[j]));  
            nodes[nodes[i].children[j]].parent = i;
        }
        if(nodes[i].children[nodes[i].child_count - 1] == n){
            break;
        }
    }
    for(int i = n; i > 0; i--){
        if(!nodes[i].sign){
            min++;
            id = find_parent(nodes, i, r);
            cover(nodes, id, r);
        }
    }
    printf("%d", min);
}