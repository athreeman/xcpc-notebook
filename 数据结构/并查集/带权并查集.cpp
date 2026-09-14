#include<bits/stdc++.h>
using namespace std;

using ll = long long;
// https://www.luogu.com.cn/problem/P8779

const int MAXN = 2e5;
int fa[MAXN + 1];
ll dist[MAXN + 1];

void init(int n) {
    for (int i = 0;i <= n;i++) {
        fa[i] = i;
        dist[i] = 0;
    }
}

int find(int u) {
    if (u != fa[u]) {
        int tmp = fa[u];
        fa[u] = find(tmp);
        dist[u] += dist[tmp];
    }
    return fa[u];
}

void merge(int u, int v, ll d) {
    int ru = find(u);
    int rv = find(v);
    if (ru != rv) {
        fa[ru] = rv;
        // u -> ru
        // v -> rv
        // 转变成: ru -> u -> v -> rv
        dist[ru] = d + dist[v] - dist[u];
    }
}

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    init(n + 1);
    for (int i = 1;i <= m;i++) {
        int l, r;ll d;
        cin >> l >> r >> d;
        r = r + 1;
        merge(l, r, d);
    }
    while (q--) {
        int l, r;
        cin >> l >> r;
        r = r + 1;
        if (find(l) != find(r)) {
            cout << "UNKNOWN" << '\n';
        }
        else {
            cout << dist[l] - dist[r] << '\n';
        }
    }
}
