#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 5e5;
//树上倍增求LCA
//预处理O(nlogn)
//单次在线查询O(logn)
//整体时间复杂度：O(nlogn)

//https://www.luogu.com.cn/problem/P3379

int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
int idx = 0;
int power = 21;
int deep[MAXN + 1];
int stjump[MAXN + 1][21];

void addEdge(int u, int v) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
}

void dfs(int cur, int fa) {
    deep[cur] = deep[fa] + 1;
    stjump[cur][0] = fa;
    for (int p = 1;p <= power;p++) {
        stjump[cur][p] = stjump[stjump[cur][p - 1]][p - 1];
    }
    for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
        if (to[edge] != fa) {
            dfs(to[edge], cur);
        }
    }
}

void buildStjump(int n, int root) {
    power = ceil(log2(n));
    dfs(root, 0);
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

void solve() {
    int n, q, root;
    cin >> n >> q >> root;
    idx = 0;
    fill(head, head + n + 1, 0);
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    buildStjump(n, root);
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << '\n';
    }
}