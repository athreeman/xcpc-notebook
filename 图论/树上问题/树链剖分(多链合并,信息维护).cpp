#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1e5;

// https://www.luogu.com.cn/problem/P2486
// 线段树区间维护 + 树链剖分合并 + 维护边界信息

array<int, 3>left, right;

int n, m;
int head[MAXN + 5];
int nxt[2 * MAXN + 5];
int to[2 * MAXN + 5];
int ei = 0;

int seg[MAXN + 5];// dfn -> 点

int dfnIdx = 0;

struct Tree {
    int color;
    int fahter;
    int dfn;
    int size;
    int son;
    int top;
    int deep;
}info[MAXN + 5];

struct SegTree {
    int sum;
    int lcolor;
    int rcolor;
    int lazy;
}tree[MAXN << 2 | 1];

void addEdge(int u, int v) {
    nxt[++ei] = head[u];
    head[u] = ei;
    to[ei] = v;
}

void lazy(int i, int to) {
    tree[i].sum = 1;
    tree[i].lcolor = to;
    tree[i].rcolor = to;
    tree[i].lazy = to;
}

void down(int i) {
    if (tree[i].lazy != -1) {
        lazy(i << 1, tree[i].lazy);
        lazy(i << 1 | 1, tree[i].lazy);
        tree[i].lazy = -1;
    }
}

void up(int i) {
    tree[i].sum = tree[i << 1].sum + tree[i << 1 | 1].sum;
    tree[i].sum = tree[i].sum - (tree[i << 1].rcolor == tree[i << 1 | 1].lcolor ? 1 : 0);
    tree[i].lcolor = tree[i << 1].lcolor;
    tree[i].rcolor = tree[i << 1 | 1].rcolor;
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
    tree[i].lazy = -1;
    if (l != r) {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        up(i);
    }
    else {
        tree[i].sum = 1;
        tree[i].lcolor = info[seg[l]].color;
        tree[i].rcolor = info[seg[l]].color;
    }
}

void update(int jobl, int jobr, int to, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        lazy(i, to);
    }
    else {
        int mid = (l + r) >> 1;
        down(i);
        if (jobl <= mid) {
            update(jobl, jobr, to, l, mid, i << 1);
        }
        if (jobr > mid) {
            update(jobl, jobr, to, mid + 1, r, i << 1 | 1);
        }
        up(i);
    }
}

int lca(int u, int v) {
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

// 查询区间[jobl, jobr]的状况
// 返回{ sum, lcolor, rcolor }
array<int, 3> query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return { tree[i].sum,tree[i].lcolor,tree[i].rcolor };
    }
    else {
        int mid = (l + r) >> 1;
        down(i);
        if (jobr <= mid) {// 左侧全包
            return query(jobl, jobr, l, mid, i << 1);
        }
        else if (jobl > mid) {
            return query(jobl, jobr, mid + 1, r, i << 1 | 1);
        }
        else {
            array<int, 3>a = query(jobl, jobr, l, mid, i << 1);
            array<int, 3>b = query(jobl, jobr, mid + 1, r, i << 1 | 1);
            array<int, 3>ans;
            ans[0] = a[0] + b[0];
            if (a[2] == b[1])ans[0]--;
            ans[1] = a[1];
            ans[2] = b[2];
            return ans;
        }
    }
}

array<int, 3> merge(array<int, 3> l, array<int, 3> r) {
    array<int, 3>ans;
    ans[0] = l[0] + r[0];
    ans[1] = l[1];
    ans[2] = r[2];
    if (l[2] == r[1]) {
        ans[0]--;
    }
    return ans;
}

int CountSeg(int u, int v) {
    ::left = { 0,0,0 };
    ::right = { 0,0,0 };
    int lc = lca(u, v);
    array<int, 3>tmp;

    while (info[u].top != info[lc].top) {
        tmp = query(info[info[u].top].dfn, info[u].dfn, 1, n, 1);
        // 注意：tmp返回的时从上往下的, 需要翻转一次才是正确的
        swap(tmp[1], tmp[2]);
        ::left = merge(::left, tmp);
        u = info[info[u].top].fahter;
    }
    tmp = query(info[lc].dfn, info[u].dfn, 1, n, 1);
    swap(tmp[1], tmp[2]);
    ::left = merge(::left, tmp);

    while (info[v].top != info[lc].top) {
        tmp = query(info[info[v].top].dfn, info[v].dfn, 1, n, 1);
        swap(tmp[1], tmp[2]);
        ::right = merge(::right, tmp);
        v = info[info[v].top].fahter;
    }
    tmp = query(info[lc].dfn, info[v].dfn, 1, n, 1);
    swap(tmp[1], tmp[2]);
    ::right = merge(::right, tmp);

    swap(::right[1], ::right[2]);

    return merge(::left, ::right)[0];
}

void ChangeRoad(int u, int v, int to) {
    while (info[u].top != info[v].top) {
        if (info[info[u].top].deep < info[info[v].top].deep) {
            swap(u, v);
        }
        update(info[info[u].top].dfn, info[u].dfn, to, 1, n, 1);
        u = info[info[u].top].fahter;
    }
    if (info[u].deep > info[v].deep) {
        swap(u, v);
    }
    update(info[u].dfn, info[v].dfn, to, 1, n, 1);
}

void solve() {
    cin >> n >> m;
    ei = dfnIdx = 0;
    for (int i = 1;i <= n;i++) {
        cin >> info[i].color;
    }
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    info[0].deep = 0;
    dfs1(1, 0);
    dfs2(1, 1);
    build(1, n, 1);
    while (m--) {
        char op;
        int a, b, c;
        cin >> op;
        if (op == 'C') {
            cin >> a >> b >> c;
            ChangeRoad(a, b, c);
        }
        else {
            cin >> a >> b;
            cout << CountSeg(a, b) << '\n';
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
