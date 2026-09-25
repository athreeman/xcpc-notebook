#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e5;
const int MAXT = (MAXN + MAXN + 2) * 26;

// 可持久化前缀树
// https://www.luogu.com.cn/problem/P6088
int version[MAXN << 2 | 1];
int tree[MAXT + 1][26];
int pass[MAXT + 1];
int end[MAXT + 1];
int space = 0;


// 核心代码如下：

int clone(int i) {
    space++;
    for (int j = 0;j < 26;j++) {
        tree[space][j] = tree[i][j];
    }
    pass[space] = pass[i];
    ::end[space] = ::end[i];
    return space;
}

int insert(string s, int i) {
    int rt = clone(i);
    pass[rt]++;
    int pre = rt, cur, path;
    for (char ch : s) {
        path = ch - 'a';
        i = tree[i][path];
        cur = clone(i);
        pass[cur]++;
        tree[pre][path] = cur;
        pre = cur;
    }
    ::end[cur]++;
    return rt;
}

int query(string s, int  i) {
    int path;
    for (char ch : s) {
        path = ch - 'a';
        i = tree[i][path];
        if (i == 0) {
            return 0;
        }
    }
    return pass[i];
}

// 图
int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
string weight[2 * MAXN + 1];
int idx = 0;
int power = 21;
int deep[MAXN + 1];
int stjump[MAXN + 1][25];

void addEdge(int u, int v, string s) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = s;
}

void dfs(int cur, int fa) {
    deep[cur] = deep[fa] + 1;
    stjump[cur][0] = fa;
    for (int p = 1;p <= power;p++) {
        stjump[cur][p] = stjump[stjump[cur][p - 1]][p - 1];
    }
    for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
        if (to[edge] != fa) {
            version[to[edge]] = insert(weight[edge], version[cur]);
            dfs(to[edge], cur);
        }
    }
}

void buildStjump(int n, int root) {
    power = ceil(log2(n));
    deep[0] = 0;
    version[0] = 0;
    version[1] = 0;
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
    int n;
    cin >> n;
    idx = space = 0;
    fill(head, head + n + 1, 0);
    for (int i = 1;i < n;i++) {
        int u, v;
        string s;
        cin >> u >> v >> s;
        addEdge(u, v, s);
        addEdge(v, u, s);
    }
    buildStjump(n, 1);
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        string s;
        cin >> s;
        cout << query(s, version[u]) + query(s, version[v]) - query(s, version[lca(u, v)]) * 2 << '\n';
    }
}
