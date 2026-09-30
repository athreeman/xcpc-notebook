#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 2e4;
const ll INF = 1e18;

// 查询区间[l, r]的上中位数
// 其中l属于[a, b], r属于[c, a]
// a < b < c < d 

// 给定区间[l, r]判断x是否为中位数:
// 我们需要维护>=x的元素个数, <x的元素个数两个信息

// 解题策略: 
// 线段树的叶节点维护的是, 当前下标的值是否>=x
// 若>=x赋值1, 否则赋值-1
// 根据这个性质, 我们通过可持久化线段树维护, 初始化全部是1
// 对排序后的数组建立, 来到位置i, 对于每个j<i的j, 把j在线段树上对应的位置标记为-1

// 更为具体的：
// 把原始数组a根据值排序，同时存储下标
// 根据排序后的数组生成每个位置的可持久化线段树
// 排序后的数组a, 对于位置i, j < i的j位置存储的下标, 线段树上标记为 - 1
// 在排序后的数组a上二分查找答案

int cnt = 0;
int n, m;
int version[MAXN + 1];
int query[4];

struct Arr {
    int val;
    int pos;
}arr[MAXN + 1];

struct tree {
    int left;
    int right;
    ll pre;
    ll suf;
    ll sum;
}node[(MAXN << 2 | 1) + MAXN * 60];

int clone(int i) {
    int rt = ++cnt;
    node[rt] = node[i];
    return rt;
}

void up(int i) {
    int l = node[i].left, r = node[i].right;
    node[i].sum = node[l].sum + node[r].sum;
    node[i].pre = max(node[l].pre, node[l].sum + node[r].pre);
    node[i].suf = max(node[r].suf, node[r].sum + node[l].suf);
}

int build(int l, int r) {
    int rt = ++cnt;
    node[rt].pre = node[rt].suf = node[rt].sum = r - l + 1;
    node[rt].left = node[rt].right = 0;
    if (l != r) {
        int mid = (l + r) >> 1;
        node[rt].left = build(l, mid);
        node[rt].right = build(mid + 1, r);
        up(rt);
    }
    return rt;
}

// 把jobi位置的值更新为-1
int update(int jobi, int l, int r, int i) {
    int rt = clone(i);
    if (l == r) {
        node[rt].pre = node[rt].suf = node[rt].sum = -1;
    }
    else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            node[rt].left = update(jobi, l, mid, node[rt].left);
        }
        else {
            node[rt].right = update(jobi, mid + 1, r, node[rt].right);
        }
        up(rt);
    }
    return rt;
}

// 查询区间[jobl, jobr]以jobl开头的最大的前缀和
array<ll, 2> queryMaxPreSum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return { node[i].pre,node[i].sum };
    }
    else {
        int mid = (l + r) >> 1;
        if (jobr <= mid) {// 完全被左侧包裹
            return queryMaxPreSum(jobl, jobr, l, mid, node[i].left);
        }
        else if (jobl > mid) {// 完全被右侧包裹
            return queryMaxPreSum(jobl, jobr, mid + 1, r, node[i].right);
        }
        else {
            array<ll, 2>left = queryMaxPreSum(jobl, jobr, l, mid, node[i].left);
            array<ll, 2>right = queryMaxPreSum(jobl, jobr, mid + 1, r, node[i].right);
            ll pre, sum;
            pre = max(left[0], left[1] + right[0]);
            sum = left[1] + right[1];
            return { pre,sum };
        }
    }
}

// 查询区间[jobl, jobr]以jobr结尾的最大后缀和
array<ll, 2> queryMaxSufSum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return { node[i].suf,node[i].sum };
    }
    else {
        int mid = (l + r) >> 1;
        if (jobl > mid) {// 完全被右侧包裹
            return queryMaxSufSum(jobl, jobr, mid + 1, r, node[i].right);
        }
        else if (jobr <= mid) {// 完全被左侧包裹
            return queryMaxSufSum(jobl, jobr, l, mid, node[i].left);
        }
        else {
            array<ll, 2>left = queryMaxSufSum(jobl, jobr, l, mid, node[i].left);
            array<ll, 2>right = queryMaxSufSum(jobl, jobr, mid + 1, r, node[i].right);
            ll suf, sum;
            suf = max(right[0], right[1] + left[0]);
            sum = left[1] + right[1];
            return { suf,sum };
        }
    }
}

// 查询区间和
ll queryRangeSum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return node[i].sum;
    }
    else {
        int mid = (l + r) >> 1;
        ll ans = 0;
        if (jobl <= mid) {
            ans += queryRangeSum(jobl, jobr, l, mid, node[i].left);
        }
        if (jobr > mid) {
            ans += queryRangeSum(jobl, jobr, mid + 1, r, node[i].right);
        }
        return ans;
    }
}


void prepare() {
    cnt = 0;
    sort(arr + 1, arr + n + 1, [](Arr& a, Arr& b) {return a.val < b.val;});
    version[1] = build(1, n);
    for (int i = 2;i <= n;i++) {
        version[i] = update(arr[i - 1].pos, 1, n, version[i - 1]);
    }
}

bool check(int a, int b, int c, int d, int pos) {
    ll sum = queryMaxSufSum(a, b, 1, n, version[pos])[0];
    sum += queryMaxPreSum(c, d, 1, n, version[pos])[0];
    sum += queryRangeSum(b + 1, c - 1, 1, n, version[pos]);
    return sum >= 0;
}

ll compute(int a, int b, int c, int d) {
    int l = 1, r = n;
    int ans = 0;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (check(a, b, c, d, mid)) {
            ans = mid;
            l = mid + 1;
        }
        else {
            r = mid - 1;
        }
    }
    return arr[ans].val;
}

// https://www.luogu.com.cn/problem/P2839


void solve() {
    cin >> n;
    for (int i = 1;i <= n;i++) {
        cin >> arr[i].val;
        arr[i].pos = i;
    }
    prepare();
    ll lastAns = 0;
    cin >> m;
    while (m--) {
        cin >> query[0] >> query[1] >> query[2] >> query[3];
        query[0] = (query[0] + lastAns) % n;
        query[1] = (query[1] + lastAns) % n;
        query[2] = (query[2] + lastAns) % n;
        query[3] = (query[3] + lastAns) % n;
        sort(query, query + 4);
        // 下标转化为1基底
        lastAns = compute(query[0] + 1, query[1] + 1, query[2] + 1, query[3] + 1);
        cout << lastAns << '\n';
    }
}
