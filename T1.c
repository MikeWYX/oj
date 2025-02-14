#include<stdio.h>
#include<string.h>
#define MAXN 150005

// 初始化所有的全局变量
int n, m, q; // n = 节点数，m = 边数，q = 查询数
int head[MAXN], to[2*MAXN], next[2*MAXN], cnt; // 原始图的邻接表，cnt是边的计数器
int dfn[MAXN], low[MAXN], tot; // Tarjan算法所需的dfn数组，low数组，tot是节点的计数器
int belong[MAXN], scc_cnt; // belong记录每个节点属于哪个强连通分量，scc_cnt是强连通分量的计数器
int scc_head[MAXN], scc_to[MAXN], scc_next[MAXN], scc_cnt2; // 强连通分量图的邻接表，scc_cnt2是边的计数器
int stack[MAXN], top; // Tarjan算法的栈，top是栈顶指针
int vis[MAXN]; // DFS访问标记

// 在原始图中添加一条从u到v的边
void add_edge(int u, int v) {
    to[++cnt] = v;
    next[cnt] = head[u];
    head[u] = cnt;
}

// 在强连通分量图中添加一条从u到v的边
void scc_add_edge(int u, int v) {
    scc_to[++scc_cnt2] = v;
    scc_next[scc_cnt2] = scc_head[u];
    scc_head[u] = scc_cnt2;
}

// Tarjan算法的主体，求原始图的强连通分量
void tarjan(int u) {
    dfn[u] = low[u] = ++tot; // 赋予每个节点一个唯一的编号
    stack[++top] = u; // 将当前节点入栈
    vis[u] = 1; // 标记该节点已入栈
    for(int i = head[u]; i; i = next[i]) { // 遍历该节点的所有出边
        int v = to[i]; // 获取该边指向的节点
        if(!dfn[v]) { // 如果这个节点未被访问过，那么就对它进行DFS
            tarjan(v);
            if(low[v] < low[u]) low[u] = low[v]; // 更新low[u]
        } else if(vis[v] && dfn[v] < low[u]) { // 如果这个节点已在栈中，那么就更新low[u]
            low[u] = dfn[v];
        }
    }
    if(dfn[u] == low[u]) { // 如果dfn[u]和low[u]相等，那么就找到了一个新的强连通分量
        scc_cnt++;
        int v;
        do {
            v = stack[top--]; // 从栈中弹出节点，直到遇到节点u
            vis[v] = 0; // 标记该节点已出栈
            belong[v] = scc_cnt; // 记录该节点属于哪个强连通分量
        } while(v != u);
    }
}

// 构建强连通分量图（有向无环图）
void build_dag() {
    for(int u = 1; u <= n; u++) {
        for(int i = head[u]; i; i = next[i]) {
            int v = to[i];
            if(belong[u] != belong[v]) { // 如果两个节点不在同一个强连通分量中，那么就在强连通分量图中添加一条边
                scc_add_edge(belong[u], belong[v]);
            }
        }
    }
}

// 在强连通分量图中进行DFS
void dfs(int u) {
    vis[u] = 1; // 标记该强连通分量已被访问
    for(int i = scc_head[u]; i; i = scc_next[i]) { // 遍历该强连通分量的所有出边
        int v = scc_to[i]; // 获取该边指向的强连通分量
        if(!vis[v]) { // 如果这个强连通分量未被访问过，那么就对它进行DFS
            dfs(v);
        }
    }
}

// 主函数
int main() {
    scanf("%d%d", &n, &m); // 读入节点数和边数
    for(int i = 1; i <= m; i++) { // 读入每条边的信息，并在原始图中添加对应的边
        int u, v;
        scanf("%d%d", &u, &v);
        add_edge(u, v);
    }
    for(int i = 1; i <= n; i++) { // 对每个节点执行Tarjan算法，求解所有的强连通分量
        if(!dfn[i]) {
            tarjan(i);
        }
    }
    build_dag(); // 构建强连通分量图

    scanf("%d", &q); // 读入查询数
    for(int i = 1; i <= q; i++) { // 对每个查询进行处理
        int a, u, v;
        scanf("%d%d%d", &a, &u, &v);
        if(!a) { // 如果查询类型为0，那么就查询u和v是否在同一个强连通分量中
            memset(vis, 0, sizeof(vis)); // 重置访问标记
            dfs(belong[u]); // 从u所在的强连通分量开始进行DFS
            if(vis[belong[v]]) { // 如果在DFS过程中访问到了v所在的强连通分量，那么就输出1，否则输出0
                printf("1\n");
            } else {
                printf("0\n");
            }
        } else { // 如果查询类型为1，那么就在原始图和强连通分量图中添加一条从u到v的边
            add_edge(u, v);
            if(belong[u] != belong[v]) {
                scc_add_edge(belong[u], belong[v]);
            }
        }
    }
    return 0;
}
