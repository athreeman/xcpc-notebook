#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 有向有权图
// dfs判断是否存在负环

const int MAXN = 3e3;
const int MAXM = 1e4;
int head[MAXN + 1];
int nxt[MAXM + 1];
int to[MAXM + 1];
ll weight[MAXM + 1];
ll value[MAXN + 1];
bool vis[MAXN + 1];
int idx = 0;

void addEdge(int u, int v, ll w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}

bool dfs(int u, int n) {
    vis[u] = true;
    if (u == 0) {
        for (int i = 1;i <= n;i++) {
            if (dfs(i, n)) {
                return true;
            }
        }
    }
    else {
        for (int edge = head[u];edge > 0;edge = nxt[edge]) {
            int v = to[edge];
            ll w = weight[edge];
            if (value[v] > value[u] + w) {
                value[v] = value[u] + w;
                if (vis[v] || dfs(v, n)) {
                    return true;
                }
            }
        }
    }
    vis[u] = false;
    return false;
}

bool check(int n) {
    // 判断是否存在负环，所有点的路径累加权值设置为0，保证只有出现负数才会更新
    fill(value, value + n + 1, 0);
    fill(vis, vis + n + 1, false);
    // 从超级源点0出发抵达每个点
    return dfs(0, n);
}
