#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 树上点和边差分，离线算法

// 根节点需要补充虚拟节点作为父亲
// 给定一棵树和树上节点u, v(u != v)
// 为u->v的路径上点的权值 + val
// 数组a是真实点权
// lc = lca(u, v)
// a[u] += val, a[v] += val, a[lc] -= val, a[fa[lca]] -= val

// 为u->v的路径上边的权值 + val
// 数组b是辅助修改边权的临时点权数组, 非真实点权
// lc = lca(u, v)
// b[u] += val, b[v] += val, b[lc] -= 2 * val
// 最后dfs遍历一遍树即可

const int MAXN = 5e4;
int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
ll a[MAXN + 1];
ll weight[2 * MAXN + 1];
ll b[MAXN + 1];
int idx = 0;
int power = 32;
int deep[MAXN + 1];
int stjump[MAXN + 1][32];

void addEdge(int u, int v, ll w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}

void dfsStjump(int cur, int fa) {
    deep[cur] = deep[fa] + 1;
    stjump[cur][0] = fa;
    for (int p = 1;p <= power;p++) {
        stjump[cur][p] = stjump[stjump[cur][p - 1]][p - 1];
    }
    for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
        if (to[edge] != fa) {
            dfsStjump(to[edge], cur);
        }
    }
}

void buildStjump(int n, int root) {
    power = ceil(log2(n));
    deep[0] = 0;
    dfsStjump(root, 0);
}

int lca(int a, int b) {
    if (deep[a] < deep[b]) {
        swap(a, b);
    }
    for (int p = power;p >= 0;p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];
        }
    }
    if (a == b) {
        return a;
    }
    for (int p = power;p >= 0;p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }
    return stjump[a][0];
}

//点差分核心代码
void dfsPoint(int u, int fa) {
    for (int v, edge = head[u];edge > 0;edge = nxt[edge]) {
        v = to[edge];
        if (v != fa) {
            dfsPoint(v, u);
            a[u] += a[v];
        }
    }
}

void addPoint(int u, int v, ll val) {
    a[u] += val;
    a[v] += val;
    int lc = lca(u, v);
    a[lc] -= val;
    a[stjump[lc][0]] -= val;
}


//边差分核心代码
void addRoad(int u, int v, ll val) {
    b[u] += val;
    b[v] += val;
    int lc = lca(u, v);
    b[lc] -= 2 * val;
}

void dfsRoad(int u, int fa) {
    for (int v, edge = head[u];edge > 0;edge = nxt[edge]) {
        v = to[edge];
        if (v != fa) {
            dfsRoad(v, u);
            b[u] += b[v];
            weight[edge] += b[v];
        }
    }
}
