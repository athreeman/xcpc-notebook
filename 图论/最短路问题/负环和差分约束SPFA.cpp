#include<bits/stdc++.h>
using namespace std;

// 差分约束两种形式
// 1、xi - yi <= ci 等价于 xi <= yi + ci
// ci是常量
// 建边yi->xi, ci是边权, 初始所有点权是(+INF)
// 通过SPFA, 不断松弛操作, 若出现负环, 无解

// 2、xi - yi >= ci 等价于 xi >= yi + ci
// ci是常量
// 建边yi->xi, ci是边权, 初始所有点权是(-INF)
// 通过SPFA, 不断扩张操作, 若出现正环, 无解

// 连通超级源点：
// 建图时需要一个超级源点root作为初始点连接所有点
// 二者可以相互转化, 区别在于松弛和扩张, 负环和正环
// ！注意！因为超级源点root的原因, 判断条件不是 >= n, 而是 > n

// 限制超级源点：
// 若出现若干变量的值已确定
// 建立一个限制超级源点limitRoot, 分别和已确定值的变量连接正负两条边
// 超级源点root和限制超级源点一定要分离

// SPFA
// 时间复杂度:O(n* m)
// n:点的个数  m:边的个数

// 模板：https://www.luogu.com.cn/problem/P5960

const int MAXN = 5e3;
using ll = long long;
const ll INF = 1e18;

int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
ll weight[2 * MAXN + 1];
int idx = 0;
int roundCnt[MAXN + 1];
ll dist[MAXN + 1];
bool vis[MAXN + 1];

void addEdge(int u, int v, ll w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}

void SPFA(int n) {
    int root = 0;
    queue<int>q;
    vis[root] = true;
    dist[root] = 0;
    roundCnt[root] = 1;
    q.push(root);
    while (q.size()) {
        int u = q.front();
        vis[u] = false;
        q.pop();
        for (int edge = head[u];edge > 0;edge = nxt[edge]) {
            int v = to[edge];
            if (dist[v] > dist[u] + weight[edge]) {
                dist[v] = dist[u] + weight[edge];
                if (!vis[v]) {
                    // ！！！注意！！！
                    // 此处因为超级源点root的原因
                    // 判断条件不是 >= n, 而是 > n
                    if (++roundCnt[v] > n) {
                        cout << "NO" << '\n';
                        return;
                    }
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
    }
    for (int i = 1;i <= n;i++) {
        cout << dist[i] << ' ';
    }
}

void solve() {
    int n, m;
    cin >> n >> m;
    for (int i = 1;i <= n;i++) {
        addEdge(0, i, 0);
        head[i] = 0;
        vis[i] = false;
        dist[i] = INF;
        roundCnt[i] = 0;
    }
    for (int i = 1;i <= m;i++) {
        ll x, y, c;
        cin >> x >> y >> c;
        // x - y <= c
        // x <= y + c
        // y->x, c
        addEdge(y, x, c);
    }
    SPFA(n);
}
