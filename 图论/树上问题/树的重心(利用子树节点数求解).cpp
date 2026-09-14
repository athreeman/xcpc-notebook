#include<bits/stdc++.h>
using namespace std;

// 树的重心：节点p
// 1、以节点p为根, 所有子树中的最大节点数最小化
// 2、以节点p为根, 每个子树的节点数都小于等于总结点数的一半(向下取整)
// 3、以节点p为根, 所有节点走向p的总边数最少
// 性质：
// 1、重心最多两个, 一定相邻
// 2、若树上增加或删除一个叶节点, 转移后的重心最多移动一条边
// 3、若把两棵树连接, 那么新的重心一定在原先重心的路径上
// 4、若树上边权都 >= 0, 一定存在所有节点走向重心的总距离和最小

//http://poj.org/problem?id=3107
const int MAXN = 5e4;
int siz[MAXN + 1];
int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
int idx = 0;
int G[3];
int maxsub[MAXN + 1];
void initial(int n) {
    idx = 0;
    fill(G, G + 3, 0);
    fill(maxsub, maxsub + n + 1, 0);
    fill(siz, siz + n + 1, 0);
    fill(head, head + n + 1, 0);
}
void addEdge(int u, int v) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
}
void dfs(int n, int u, int fa) {
    siz[u] = 1;
    for (int edge = head[u];edge > 0;edge = nxt[edge]) {
        if (to[edge] != fa) {
            dfs(n, to[edge], u);
            siz[u] += siz[to[edge]];
            maxsub[u] = max(maxsub[u], siz[to[edge]]);
        }
    }
    maxsub[u] = max(maxsub[u], n - siz[u]);
}
void solve() {
    int n;
    cin >> n;
    initial(n);
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs(n, 1, 0);
    for (int i = 1;i <= n;i++) {
        if (maxsub[i] <= n / 2) {
            G[++G[0]] = i;
        }
    }
    for (int i = 1;i <= G[0];i++) {
        cout << G[i] << ' ';
    }
    cout << '\n';
}
