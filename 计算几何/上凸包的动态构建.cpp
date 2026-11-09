#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2e5;

struct Point { ll x, y; };

int m = 0;

Point hull[MAXN + 1];

Point minus(Point a, Point b) {
    return { a.x - b.x,
             a.y - b.y };
}

ll cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}


// 要求：插入顺序, 横坐标必须递增

// 上凸包:
// 递增 + 大于等于
// 递减 + 小于等于

// 下凸包:
// 递增 + 小于等于
// 递减 + 大于等于

void insert(Point a) {
    while (m >= 2 &&
        cross(::minus(hull[m], hull[m - 1]), ::minus(a, hull[m - 1])) >= 0) {
        m--;
    }
    hull[++m] = a;
}