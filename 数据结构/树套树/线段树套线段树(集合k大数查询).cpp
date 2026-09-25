#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 建议函数名附带outer, inner提高可读性
// 线段树套线段树, 此处采用不同入参的嵌套函数写法

// https://www.luogu.com.cn/problem/P3332

const int MAXN = 5e4;
const int MAXT = MAXN * 300;

struct Tree {
    int left;
    int right;
    ll lazy;
    ll cnt;
    int innerRoot;// 内层线段树的头
}node[MAXT + 1];
// space必须初始化为1, 1作为外层的头
int space = 1;

int create() {
    space++;
    node[space] = { 0,0,0,0,0 };
    return space;
}

int val[MAXN + 1];
int vi = 0;
int n, m;

ll que[MAXN + 1][4];

void prepare() {
    sort(val + 1, val + vi + 1);
    val[0] = -1e9;
    int k = vi;
    vi = 0;
    for (int i = 1;i <= k;i++) {
        if (val[vi] != val[i]) {
            val[++vi] = val[i];
        }
    }
}

int id(int x) {
    int l = 1, r = vi;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (val[mid] == x) {
            return mid;
        }
        else if (val[mid] < x) {
            l = mid + 1;
        }
        else {
            r = mid - 1;
        }
    }
    return l;
}

void lazy(int i, ll siz, ll jobv) {
    if (i == 0)i = create();
    node[i].lazy += jobv;
    node[i].cnt += siz * jobv;
}

void down(int i, ll ln, ll rn) {
    if (node[i].lazy > 0) {
        if (node[i].left == 0)node[i].left = create();
        if (node[i].right == 0)node[i].right = create();
        lazy(node[i].left, ln, node[i].lazy);
        lazy(node[i].right, rn, node[i].lazy);
        node[i].lazy = 0;
    }
}

// 在某个内层线段树, [jobl, jobr]集合各个词频 + 1
int insert(int jobl, int jobr, int l, int r, int i) {
    if (i == 0)i = create();
    if (jobl <= l && r <= jobr) {
        lazy(i, r - l + 1, 1);
    }
    else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);
        if (jobl <= mid) {
            node[i].left = insert(jobl, jobr, l, mid, node[i].left);
        }
        if (jobr > mid) {
            node[i].right = insert(jobl, jobr, mid + 1, r, node[i].right);
        }
        node[i].cnt = node[node[i].left].cnt + node[node[i].right].cnt;
    }
    return i;
}

// 在集合[jobl, jobr]插入值jobv
int insert(int jobv, int jobl, int jobr, int l, int r, int i) {
    if (i == 0)i = create();
    if (l == r) {
        node[i].innerRoot = insert(jobl, jobr, 1, n, node[i].innerRoot);
    }
    else {
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            node[i].left = insert(jobv, jobl, jobr, l, mid, node[i].left);
        }
        else {
            node[i].right = insert(jobv, jobl, jobr, mid + 1, r, node[i].right);
        }
        node[i].innerRoot = insert(jobl, jobr, 1, n, node[i].innerRoot);
    }
    return i;
}

// 查询集合[jobl, jobr]的词频和
ll query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return node[i].cnt;
    }
    else {
        ll ans = 0;
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);
        if (jobl <= mid) {
            ans += query(jobl, jobr, l, mid, node[i].left);
        }
        if (jobr > mid) {
            ans += query(jobl, jobr, mid + 1, r, node[i].right);
        }
        return ans;
    }
}

// 查询位于集合[jobl, jobr]的第jobk大的元素
ll findKth(int jobl, int jobr, ll jobk, int l, int r, int i) {
    if (i == 0)return 0;
    if (l == r) {
        return l;
    }
    else {
        int mid = (l + r) >> 1;
        ll cnt = query(jobl, jobr, 1, n, node[node[i].right].innerRoot);
        if (cnt >= jobk) {
            return findKth(jobl, jobr, jobk, mid + 1, r, node[i].right);
        }
        else {
            return findKth(jobl, jobr, jobk - cnt, l, mid, node[i].left);
        }
    }
}

void solve() {
    cin >> n >> m;
    vi = 0;
    node[1] = { 0,0,0,0,0 };
    for (int i = 1;i <= m;i++) {
        cin >> que[i][0] >> que[i][1] >> que[i][2] >> que[i][3];
        if (que[i][0] == 1) {
            val[++vi] = que[i][3];
        }
    }
    prepare();
    for (int i = 1;i <= m;i++) {
        if (que[i][0] == 2) {
            int l = que[i][1], r = que[i][2], k = que[i][3];
            int pos = findKth(l, r, k, 1, vi, 1);
            cout << val[pos] << '\n';
        }
        else {
            insert(id(que[i][3]), que[i][1], que[i][2], 1, vi, 1);
        }
    }
}
