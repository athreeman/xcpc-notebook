#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// https://www.luogu.com.cn/problem/P3384
// 注意, 该例题需要取模运算(否则WA), 模板代码省去了这一部分

// 树链剖分的应用核心：
// 通过把树拆成链, 以及dfn序的维护
// 把树的结构转化为连续的序列
// 从而更具备灵活性, 可与其他算法结合(例如线段树)

// 维护信息：
// size: 子树大小
// deep: 节点深度
// father: 直接父节点
// son: 大小最大的儿子
// top: 所在重链的头
// dfn: 递归序
// seg: dfn序对点的映射
// size, deep, father, son在第一次dfs时维护
// dfn, seg, top在第二次dfs遍历时维护

const int MAXN = 1e5;

int n, m, root, mod;
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

struct SegTree {
    ll sum;
    ll lazy;
}tree[MAXN << 2 | 1];

void lazy(int i, int siz, ll jobv) {
    tree[i].sum = (tree[i].sum + (ll)siz * jobv);
    tree[i].lazy = (tree[i].lazy + jobv);
}

void down(int i, int lsiz, int rsiz) {
    if (tree[i].lazy != 0) {
        lazy(i << 1, lsiz, tree[i].lazy);
        lazy(i << 1 | 1, rsiz, tree[i].lazy);
        tree[i].lazy = 0;
    }
}

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

void build(int l, int r, int i) {
    tree[i].lazy = 0;
    if (l == r) {
        tree[i].sum = info[seg[l]].power;
    }
    else {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        tree[i].sum = (tree[i << 1].sum + tree[i << 1 | 1].sum);
    }
}

void add(int jobl, int jobr, ll jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        lazy(i, r - l + 1, jobv);
    }
    else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        tree[i].sum = (tree[i << 1].sum + tree[i << 1 | 1].sum);
    }
}

ll query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return tree[i].sum;
    }
    else {
        int mid = (l + r) >> 1;
        ll ans = 0;
        down(i, mid - l + 1, r - mid);
        if (jobl <= mid) {
            ans = (ans + query(jobl, jobr, l, mid, i << 1));
        }
        if (jobr > mid) {
            ans = (ans + query(jobl, jobr, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

void addSubTree(int u, ll jobv) {
    add(info[u].dfn, info[u].dfn + info[u].size - 1, jobv, 1, n, 1);
}

void addRoadUtoV(int u, int v, ll jobv) {
    int h;
    while (info[u].top != info[v].top) {
        if (info[info[u].top].deep < info[info[v].top].deep) {
            swap(u, v);
        }
        h = info[u].top;
        add(info[h].dfn, info[u].dfn, jobv, 1, n, 1);
        u = info[h].fahter;
    }
    if (info[u].deep > info[v].deep) {
        swap(u, v);
    }
    add(info[u].dfn, info[v].dfn, jobv, 1, n, 1);
}

ll querySubTree(int u) {
    return query(info[u].dfn, info[u].dfn + info[u].size - 1, 1, n, 1);
}

ll queryRoadUtoV(int u, int v) {
    ll ans = 0;
    int h;
    while (info[u].top != info[v].top) {
        if (info[info[u].top].deep < info[info[v].top].deep) {
            swap(u, v);
        }
        h = info[u].top;
        ans = (ans + query(info[h].dfn, info[u].dfn, 1, n, 1));
        u = info[h].fahter;
    }
    if (info[u].deep > info[v].deep) {
        swap(u, v);
    }
    ans = (ans + query(info[u].dfn, info[v].dfn, 1, n, 1));
    return ans;
}

void solve() {
    cin >> n >> m >> root >> mod;
    for (int i = 1;i <= n;i++) {
        cin >> info[i].power;
    }
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs1(root, 0);
    dfs2(root, root);
    build(1, n, 1);
    while (m--) {
        int op;
        int x, y, z;
        cin >> op;
        if (op == 1) {
            cin >> x >> y >> z;
            addRoadUtoV(x, y, z);
        }
        else if (op == 2) {
            cin >> x >> y;
            cout << queryRoadUtoV(x, y) << '\n';
        }
        else if (op == 3) {
            cin >> x >> z;
            addSubTree(x, z);
        }
        else {
            cin >> x;
            cout << querySubTree(x) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
