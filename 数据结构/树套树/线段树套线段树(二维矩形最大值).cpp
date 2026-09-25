#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 建议函数名附带outer, inner提高可读性
// 线段树套线段树, 此处采用不同入参的嵌套函数写法
// https://acm.hdu.edu.cn/showproblem.php?pid=1823

const int MAXH = 105;
const int MAXN = 1005;
// 第一维 [0, 100] 第二维 [0, 1000]
int tree[MAXH << 2 | 1][MAXN << 2 | 1] = {};

// 在某个内部线段树, 在活泼度为jobA的位置, 插入缘分值jobL
void insert(int* a, int jobA, int jobL, int l, int r, int i) {
    if (l == r) {
        a[i] = max(a[i], jobL);
    }
    else {
        int mid = (l + r) >> 1;
        if (jobA <= mid) {
            insert(a, jobA, jobL, l, mid, i << 1);
        }
        else {
            insert(a, jobA, jobL, mid + 1, r, i << 1 | 1);
        }
        a[i] = max(a[i << 1], a[i << 1 | 1]);
    }
}

// 插入身高包含H的内部树, 活泼度为A, 缘分值为L
void insert(int a[][MAXN << 2 | 1], int jobH, int jobA, int jobL, int l, int r, int i) {
    if (l == r) {
        insert(a[i], jobA, jobL, 0, 1000, 1);
    }
    else {
        int mid = (l + r) >> 1;
        if (jobH <= mid) {
            insert(a, jobH, jobA, jobL, l, mid, i << 1);
        }
        else {
            insert(a, jobH, jobA, jobL, mid + 1, r, i << 1 | 1);
        }
        insert(a[i], jobA, jobL, 0, 1000, 1);
    }
}

// 在某个内部线段树, 查询活泼度在[jobl, jobr]的最大缘分值
int query(int* a, int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return a[i];
    }
    else {
        int ans = -1;
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            ans = max(ans, query(a, jobl, jobr, l, mid, i << 1));
        }
        if (jobr > mid) {
            ans = max(ans, query(a, jobl, jobr, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

// 查询身高在[hl, hr]范围, 活泼度在[al, ar]范围内, 最高的缘分值
int query(int a[][MAXN << 2 | 1], int hl, int hr, int al, int ar, int l, int r, int i) {
    if (hl <= l && r <= hr) {
        return query(a[i], al, ar, 0, 1000, 1);
    }
    else {
        int ans = -1;
        int mid = (l + r) >> 1;
        if (hl <= mid) {
            ans = max(ans, query(a, hl, hr, al, ar, l, mid, i << 1));
        }
        if (hr > mid) {
            ans = max(ans, query(a, hl, hr, al, ar, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

void solve(int m) {
    memset(tree, -1, sizeof(tree));
    while (m--) {
        char op;
        cin >> op;
        if (op == 'I') {
            int h;
            double a, l;
            cin >> h >> a >> l;
            insert(tree, h - 100, a * 10.0, l * 10.0, 0, 100, 1);
        }
        else {
            int h1, h2;
            double a1, a2;
            cin >> h1 >> h2 >> a1 >> a2;
            if (h1 > h2)swap(h1, h2);
            if (a1 > a2)swap(a1, a2);
            double ans = query(tree, h1 - 100, h2 - 100, a1 * 10.0, a2 * 10.0, 0, 100, 1);
            if (ans < 0) {
                cout << -1 << '\n';
            }
            else {
                ans = ans / 10.0;
                cout << fixed << setprecision(1) << ans << '\n';
            }
        }
    }
}
