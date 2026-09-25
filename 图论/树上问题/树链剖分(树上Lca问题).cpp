#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 5e5;

// https://www.luogu.com.cn/problem/P3379

int n, m, root;
int head[MAXN + 5];
int nxt[2 * MAXN + 5];
int to[2 * MAXN + 5];
int ei = 0;

int seg[MAXN + 5];// dfn -> 点

int dfnIdx = 0;

struct Tree {
    int power;
    int fahter;
    int dfn;
    int size;
    int son;
    int top;
    int deep;
}info[MAXN + 5];

void addEdge(int u, int v) {
    nxt[++ei] = head[u];
    head[u] = ei;
    to[ei] = v;
}

void dfs1(int u, int f) {
    info[u].fahter = f;
    info[u].size = 1;
    info[u].deep = info[f].deep + 1;
    for (int v, edge = head[u];edge > 0;edge = nxt[edge]) {
        v = to[edge];
        if (v != f) {
            dfs1(v, u);
            info[u].size += info[v].size;
            if (info[info[u].son].size < info[v].size) {
                info[u].son = v;
            }
        }
    }
}

void dfs2(int u, int h) {
    info[u].top = h;
    info[u].dfn = ++dfnIdx;
    seg[info[u].dfn] = u;
    if (info[u].son != 0) {
        dfs2(info[u].son, h);
        for (int v, edge = head[u];edge > 0;edge = nxt[edge]) {
            v = to[edge];
            if (v != info[u].fahter && v != info[u].son) {
                dfs2(v, v);
            }
        }
    }
}

int lca(int u, int v) {
    int h;
    while (info[u].top != info[v].top) {
        if (info[info[u].top].deep < info[info[v].top].deep) {
            swap(u, v);
        }
        u = info[info[u].top].fahter;
    }
    if (info[u].deep > info[v].deep) {
        swap(u, v);
    }
    return u;
}

void solve() {
    cin >> n >> m >> root;
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs1(root, 0);
    dfs2(root, 0);
    for (int i = 1;i <= m;i++) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << '\n';
    }
}
