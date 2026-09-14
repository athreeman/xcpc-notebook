#include<bits/stdc++.h>
using namespace std;
const int MAXN = 5e5;

//离线处理操作
//离线处理：O(n)
//单次离线查询：O(1)
//整体复杂度：O(n)
//核心代码：dfs+并查集
//模板例题：https://www.luogu.com.cn/problem/P3379

int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
int idx = 0;
int ans[MAXN + 1];
vector<array<int, 2>>query[MAXN + 1];
int fa[MAXN + 1];
bool vis[MAXN + 1];
int find(int u) {
    return fa[u] == u ? u : fa[u] = find(fa[u]);
}

void addEdge(int u, int v) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
}

void tarjan(int cur, int father) {
    // ！！！一旦进入，立刻标记已抵达
    vis[cur] = true;
    for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
        if (to[edge] != father) {
            tarjan(to[edge], cur);
            //！！！注意！！！
            //！！！设置父节点必须写在子节点递归回溯后！！！
            fa[to[edge]] = cur;
        }
    }
    // ！！！子节点遍历完，再查询答案
    int a = cur;
    for (auto& [b, i] : query[a]) {
        if (vis[b]) {
            ans[i] = find(b);
        }
    }
}

void solve() {
    int n, q, root;
    cin >> n >> q >> root;
    idx = 0;
    fill(head, head + n + 1, 0);
    fill(vis, vis + n + 1, 0);
    for (int i = 1;i <= n;i++) {
        fa[i] = i;
    }
    for (int i = 1;i < n;i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1;i <= q;i++) {
        int a, b;
        cin >> a >> b;
        query[a].push_back({ b,i });
        query[b].push_back({ a,i });
    }
    tarjan(root, 0);
    for (int i = 1;i <= q;i++) {
        cout << ans[i] << '\n';
    }
}
