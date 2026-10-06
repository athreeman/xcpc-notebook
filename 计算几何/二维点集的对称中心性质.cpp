#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 2e5;

int n;
struct Point {
    ll x, y;
}point[MAXN + 1];

// 给定n个互不相同点的点集S
// 定义G: 这n个点的重心
// 如果存在点P使得每个点p, 都存在一个点q满足, p + q = 2 * P
// 则称点集S中心对称, 一定存在中心对称点P = G

// 性质：

// 1、
// 如果点集S关于P中心对称, 则根据字典序(x和y升序)排序后
// 字典序最小的点A, 最大的点B, 一定满足2 * P = A + B

// 2、所有点对(无序对)的平方距离和(不开根号), 即所有点到重心G的距离和

// 3、重心G是平面上所有点, 到点集S所有点平方距离和(不开根)最小的点

bool checkCenter(int n) {
    Point G;
    G.x = G.y = 0;
    map<array<ll, 2>, bool>ishas;
    for (int i = 1;i <= n;i++) {
        G.x += point[i].x;
        G.y += point[i].y;
        ishas[{ll(n)* point[i].x, (ll)n* point[i].y}] = true;
    }
    G.x = G.x * 2LL;
    G.y = G.y * 2LL;
    for (int i = 1;i <= n;i++) {
        if (!ishas.count({ G.x - point[i].x * (ll)n, G.y - point[i].y * (ll)n })) {
            return false;
        }
    }
    return true;
}