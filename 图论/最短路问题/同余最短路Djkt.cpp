#include<bits/stdc++.h>
using namespace std;
using ll = long long;


// https://www.luogu.com.cn/problem/P3403

// 解决数据量大的完全背包问题
// 使用特定数值的余数分组
// 注意：按照余数分组[0, x - 1], 所以是以0为基准而非1
// 根据题目不同可能需要对背包容量进行定制, 例如n = n - 1
// 最终计算的dist[i]即 % base = i可达到的最低值

const int MAXN = 1e5;
const ll INF = LLONG_MAX;
int head[MAXN + 1];
int nxt[2 * MAXN + 1];
int to[2 * MAXN + 1];
ll weight[2 * MAXN + 1];
int idx = 0;
ll dist[MAXN + 1];
bool vis[MAXN + 1];

void addEdge(int u, int v, ll w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}

void Djk() {
    dist[0] = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>>q;
    q.push({ 0,0 });
    while (q.size()) {
        auto [d, u] = q.top();
        q.pop();
        if (vis[u])continue;
        dist[u] = d;
        vis[u] = true;
        for (int edge = head[u];edge > 0;edge = nxt[edge]) {
            int v = to[edge];
            if (!vis[v] && dist[v] > dist[u] + (ll)weight[edge]) {
                dist[v] = dist[u] + weight[edge];
                q.push({ dist[v],v });
            }
        }
    }
}

void solve() {
    ll h, x, y, z;
    cin >> h >> x >> y >> z;
    h = h - 1;
    // 让最小值成为x, 减少开销
    if (y > z)swap(y, z);
    if (x > y)swap(x, y);
    fill(dist, dist + x + 1, INF);
    for (int i = 0;i < x;i++) {
        addEdge(i, (i + y) % x, y);
        addEdge(i, (i + z) % x, z);
    }
    Djk();
    ll ans = 0;
    for (int i = 0;i < x;i++) {
        if (dist[i] <= h) {
            ans += (h - dist[i]) / x + 1;
        }
    }
    cout << ans << '\n';
}
