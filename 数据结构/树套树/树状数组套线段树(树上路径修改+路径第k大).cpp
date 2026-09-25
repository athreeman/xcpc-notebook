#include<bits/stdc++.h>
namespace name {
    using std::ios;
    using std::cout;
    using std::cin;
    using std::fill;
    using std::vector;
    using std::array;
    using std::sort;
}
using namespace name;
using ll = long long;

const int MAXN = 8e4;
const int MAXT = MAXN * 300;

// https://www.luogu.com.cn/problem/P4175
// 解题核心：
// 静态可持久化线段树维护初始信息
// 使用差分树状数组套开点线段树维护动态可修改信息
// 查询时, 二者的累加和就是答案

int n, q;

vector<array<int, 2>>lca[MAXN | 1];
bool vis[MAXN | 1];

int fa[MAXN | 1];
int find(int u) {
    return u == fa[u] ? u : fa[u] = find(fa[u]);
}

// 树状数组套线段树, 维护差分信息

int space = 0;
int innerRoot[MAXN | 1];
int tree[MAXT | 1];
int left[MAXT | 1];
int right[MAXT | 1];

// 离散化数组
int val[MAXN << 1 | 1];
int vi = 0;

int dist[MAXN | 1];
int father[MAXN | 1];
int dfn[MAXN + 2];
int siz[MAXN + 2];
int power[MAXN + 2];
int version[MAXN + 2];
int dfnIdx = 0;


int idx = 0;
int head[MAXN + 1];
int nxt[MAXN << 1 | 1];
int to[MAXN << 1 | 1];

int que[MAXN + 1][4];

int lowbit(int x) {
    return x & -x;
}

int create() {
    space++;
    tree[space] = 0;
    return space;
}

int addTree[MAXN | 1];
int ai = 0;
int minusTree[MAXN | 1];
int mi = 0;

// 维护静态可持久化线段树
struct Tree {
    int left;
    int right;
    int cnt;
}node[MAXT | 1];
int Ti = 0;

int clone(int i) {
    node[++Ti] = node[i];
    return Ti;
}

int id(int x) {
    int l = 1, r = vi;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (val[mid] == x) {
            return mid;
        }
        else if (val[mid] > x) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    return l;
}

// 基于i版本, 在位置jobi词频+1, 并返回新的头部
int add(int jobi, int l, int r, int i) {
    i = clone(i);
    if (l == r) {
        node[i].cnt++;
    }
    else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            node[i].left = add(jobi, l, mid, node[i].left);
        }
        else {
            node[i].right = add(jobi, mid + 1, r, node[i].right);
        }
        node[i].cnt = node[node[i].left].cnt + node[node[i].right].cnt;
    }
    return i;
}

// 初始化
// 构建dfn序, 子树大小siz, 静态信息(可持久化线段树)
void init(int u, int f) {
    father[u] = f;
    dist[u] = dist[f] + 1;
    int i = ++dfnIdx;
    vis[u] = true;
    dfn[u] = i;
    siz[i] = 1;
    version[u] = add(id(power[u]), 1, vi, version[f]);
    for (int v, edge = head[u];edge > 0;edge = nxt[edge]) {
        v = to[edge];
        if (v != f) {
            init(v, u);
            siz[i] += siz[dfn[v]];
            fa[v] = u;
        }
    }
    int a = u;
    for (auto& [b, i] : lca[a]) {
        if (vis[b]) {
            que[i][3] = find(b);
        }
    }
}


int update(int jobi, int jobv, int l, int r, int i) {
    if (i == 0)i = create();
    if (l == r) {
        tree[i] += jobv;
    }
    else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            left[i] = update(jobi, jobv, l, mid, left[i]);
        }
        else {
            right[i] = update(jobi, jobv, mid + 1, r, right[i]);
        }
        tree[i] = tree[left[i]] + tree[right[i]];
    }
    return i;
}

void add(int i, int jobv, int v) {
    while (i <= n) {
        innerRoot[i] = update(id(jobv), v, 1, vi, innerRoot[i]);
        i += lowbit(i);
    }
}

void addEdge(int u, int v) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
}

// 离散化+初始化
void prepare() {
    sort(val + 1, val + vi + 1);
    int k = vi;
    vi = 0;
    val[0] = -1e9;
    for (int i = 1;i <= k;i++) {
        if (val[vi] != val[i]) {
            val[++vi] = val[i];
        }
    }
    version[0] = 0;
    innerRoot[0] = 0;
    dist[0] = 1;
    dfn[0] = 0;
    init(1, 0);
}

void moveLeft() {
    for (int i = 1;i <= ai;i++) {
        addTree[i] = left[addTree[i]];
    }
    for (int i = 1;i <= mi;i++) {
        minusTree[i] = left[minusTree[i]];
    }
}

void moveRight() {
    for (int i = 1;i <= ai;i++) {
        addTree[i] = right[addTree[i]];
    }
    for (int i = 1;i <= mi;i++) {
        minusTree[i] = right[minusTree[i]];
    }
}

int Count() {
    int ans = 0;
    for (int i = 1;i <= ai;i++) {
        ans += tree[left[addTree[i]]];
    }
    for (int i = 1;i <= mi;i++) {
        ans -= tree[left[minusTree[i]]];
    }
    return ans;
}

int findKth(int jobk, int l, int r, int a, int b, int lc, int lcf) {
    if (l == r) {
        return l;
    }
    else {
        int mid = (l + r) >> 1;
        int lsiz = node[node[a].left].cnt + node[node[b].left].cnt - node[node[lc].left].cnt - node[node[lcf].left].cnt;
        // 计算lsiz
        lsiz += Count();
        if (lsiz >= jobk) {
            moveLeft();
            return findKth(jobk, l, mid, node[a].left, node[b].left, node[lc].left, node[lcf].left);
        }
        else {
            moveRight();
            return findKth(jobk - lsiz, mid + 1, r, node[a].right, node[b].right, node[lc].right, node[lcf].right);
        }
    }
}

int compute(int a, int b, int lc, int lcf, int jobk) {
    ai = 0;
    mi = 0;
    int i = dfn[a];
    while (i > 0) {
        addTree[++ai] = innerRoot[i];
        i -= lowbit(i);
    }
    i = dfn[b];
    while (i > 0) {
        addTree[++ai] = innerRoot[i];
        i -= lowbit(i);
    }
    i = dfn[lc];
    while (i > 0) {
        minusTree[++mi] = innerRoot[i];
        i -= lowbit(i);
    }
    i = dfn[lcf];
    while (i > 0) {
        minusTree[++mi] = innerRoot[i];
        i -= lowbit(i);
    }
    return val[findKth(jobk, 1, vi, version[a], version[b], version[lc], version[lcf])];
}

void solve() {
    Ti = 0;
    dfnIdx = 0;
    space = 0;
    idx = 0;
    vi = 0;
    cin >> n >> q;
    fill(head, head + n + 1, 0);
    fill(vis, vis + n + 1, false);
    for (int i = 0;i <= n;i++) {
        fa[i] = i;
    }
    for (int i = 1;i <= n;i++) {
        cin >> power[i];
        val[++vi] = power[i];
    }
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1;i <= q;i++) {
        cin >> que[i][0] >> que[i][1] >> que[i][2];
        if (que[i][0] == 0) {
            val[++vi] = que[i][2];
        }
        else {
            lca[que[i][1]].push_back({ que[i][2],i });
            lca[que[i][2]].push_back({ que[i][1],i });
        }
    }
    prepare();
    int k, a, b;
    for (int i = 1;i <= q;i++) {
        k = que[i][0];
        a = que[i][1];
        b = que[i][2];
        if (k == 0) {
            int l = dfn[a];
            int r = dfn[a] + siz[dfn[a]] - 1;
            int val = power[a];
            add(l, val, -1);
            add(r + 1, val, 1);
            add(l, b, 1);
            add(r + 1, b, -1);
            power[a] = b;
        }
        else {
            int lc = que[i][3];
            int lcf = father[lc];
            // 距离 + 1就是点的个数
            int c = dist[a] + dist[b] - dist[lc] * 2 + 1;
            if (c < k) {
                cout << "invalid request!" << '\n';
            }
            else {
                k = c - k + 1;
                cout << compute(a, b, lc, lcf, k) << '\n';
            }
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
