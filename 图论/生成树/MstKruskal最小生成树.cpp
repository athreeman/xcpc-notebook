#include<bits/stdc++.h>
using namespace std;
using ll = long long;

//最小生成树,Kruskal算法
//根据边权从小到大排序
//若连接当前边不会形成环就连接,否则跳过
//达到n-1条边完成
//复杂度取决于边的个数
//点n,边m,时间复杂度O(mlogn)

//模板例题：https://www.luogu.com.cn/problem/P3366

const int MAXN = 5e3;
const int MAXM = 2e5;

array<int, 3> edge[MAXM + 1];

//并查集
int father[MAXN + 1];
int siz[MAXN + 1];
int road[MAXN + 1];

int find(int u) {
    int cnt = -1;
    while (u != father[u]) {
        road[++cnt] = u;
        u = father[u];
    }
    while (cnt >= 0) {
        father[road[cnt]] = u;
        cnt--;
    }
    return u;
}

bool merge(int u, int v) {
    int ru = find(u), rv = find(v);
    if (ru == rv)return false;
    if (siz[ru] <= siz[rv]) {
        father[ru] = rv;
        siz[rv] += siz[ru];
    }
    else {

        father[rv] = ru;
        siz[ru] += siz[rv];
    }
    return true;
}

void initial(int n) {
    //略......
}

bool cmp(array<int, 3>a, array<int, 3>b) {
    return a[2] < b[2];
}

#include<bits/stdc++.h>
using namespace std;
using ll = long long;

//最小生成树,Kruskal算法
//根据边权从小到大排序
//若连接当前边不会形成环就连接,否则跳过
//达到n-1条边完成
//复杂度取决于边的个数
//点n,边m,时间复杂度O(mlogn)

//模板例题：https://www.luogu.com.cn/problem/P3366

const int MAXN = 5e3;
const int MAXM = 2e5;

array<int, 3> edge[MAXM + 1];

//并查集
int father[MAXN + 1];
int siz[MAXN + 1];
int road[MAXN + 1];

int find(int u) {
    int cnt = -1;
    while (u != father[u]) {
        road[++cnt] = u;
        u = father[u];
    }
    while (cnt >= 0) {
        father[road[cnt]] = u;
        cnt--;
    }
    return u;
}

bool merge(int u, int v) {
    int ru = find(u), rv = find(v);
    if (ru == rv)return false;
    if (siz[ru] <= siz[rv]) {
        father[ru] = rv;
        siz[rv] += siz[ru];
    }
    else {

        father[rv] = ru;
        siz[ru] += siz[rv];
    }
    return true;
}

void initial(int n) {
    //略......
}

bool cmp(array<int, 3>a, array<int, 3>b) {
    return a[2] < b[2];
}
void solve() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    initial(n);
    for (int i = 1;i <= m;i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edge[i][0] = u;
        edge[i][1] = v;
        edge[i][2] = w;
    }
    sort(edge + 1, edge + m + 1, cmp);
    ll ans = 0;
    ll cnt = 0;
    for (int i = 1;i <= m;i++) {
        int u = edge[i][0], v = edge[i][1];
        ll w = edge[i][2];
        if (merge(u, v)) {
            ans += w;
            cnt++;
        }
        if (cnt == n - 1)break;
    }
    if (cnt == n - 1) {
        cout << ans << '\n';
    }
    else {
        cout << "orz" << '\n';
    }
}
