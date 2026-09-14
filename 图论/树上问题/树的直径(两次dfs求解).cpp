#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 树的直径：树上距离最远的两个点，形成的路径
// 性质：基于边权都 >= 0(非负)
// 1、若有多条直径，则他们一定存在共有的中间部分(一个点或一段路径)
// 2、对于树上任意点p，与点p相隔最远的点的集合，直径的两端至少有一个在里面

// 树合并,直径边权最短问题：
// 条件：要求所有边权非负且完全相等
// 两颗树, 直径上边的数量分别是A,B,则合并后最长直径=A+B+1,最短=max(A,B,A/2+B/2+1),除法分别向上取整

// 解法：
// 1、两次dfs，可以得到直径、沿途点....等信息
// 条件：边权非负
// 2、树型dp，只能得到直径
// 条件：适用任何树

const int MAXN = 5e5;
int head[MAXN + 1];
int to[2 * MAXN + 1];
int nxt[2 * MAXN + 1];
ll weight[2 * MAXN + 1];
int lst[MAXN + 1];//路径的父节点
ll dist[MAXN + 1];
int idx = 0;
int st, ed;ll diameter;
void addEdge(int u, int v, ll w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}
void initial(int n) {
    idx = 0;
    fill(head, head + n + 1, 0);
}
void dfs(int u, int fa) {
    lst[u] = fa;
    for (int edge = head[u];edge > 0;edge = nxt[edge]) {
        if (to[edge] != fa) {
            dist[to[edge]] = dist[u] + weight[edge];
            dfs(to[edge], u);
        }
    }
}
void GetMaxRoad(int n) {
    dist[1] = 0;
    dfs(1, 0);
    st = 1;
    for (int i = 2;i <= n;i++) {
        if (dist[i] > dist[st]) {
            st = i;
        }
    }
    fill(lst, lst + n + 1, 0);
    dist[st] = 0;
    dfs(st, 0);
    ed = 1;
    for (int i = 2;i <= n;i++) {
        if (dist[i] > dist[ed]) {
            ed = i;
        }
    }
    diameter = dist[ed];
}
void solve() {
    int n;
    cin >> n;
    initial(n);
    for (int i = 1;i < n;i++) {
        int u, v;ll w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    GetMaxRoad(n);
    cout << st << "->" << ed << ' ' << diameter << '\n';
}
