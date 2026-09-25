#include<bits/stdc++.h>
using namespace std;

// 树状数组套线段树
// 本质上可以理解为：动态可修改的主席树

// https://www.luogu.com.cn/problem/P3380

const int MAXN = 2e5;
const int MAXT = MAXN * 300;

int lowbit(int x) {
    return x & -x;
}

int innerRoot[MAXN + 1];

// 线段树
struct Tree {
    int left;
    int right;
    int cnt;
}node[MAXT + 1];
int space = 0;
int n, m;

int val[MAXN << 1 | 1];
int vi = 0;

int addTree[MAXN + 1];
int minusTree[MAXN + 1];
int ai = 0, mi = 0;

int arr[MAXN + 1];
int que[MAXN + 1][4];

int id(int x) {
    int l = 1, r = vi;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (val[mid] == x) {
            return mid;
        }
        else if (val[mid] > x) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    return l;
}

int create() {
    space++;
    node[space] = { 0,0,0 };
    return space;
}

void up(int i) {
    node[i].cnt = node[node[i].left].cnt + node[node[i].right].cnt;
}

// 在某个内层线段树, 位置jobi词频增加jobv
int insert(int jobi, int jobv, int l, int r, int i) {
    if (i == 0)i = create();
    if (l == r) {
        node[i].cnt += jobv;
    }
    else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            node[i].left = insert(jobi, jobv, l, mid, node[i].left);
        }
        else {
            node[i].right = insert(jobi, jobv, mid + 1, r, node[i].right);
        }
        up(i);
    }
    return i;
}

// 下标i位置, val的词频增加v
void add(int i, int val, int v) {
    while (i <= n) {
        innerRoot[i] = insert(id(val), v, 1, vi, innerRoot[i]);
        i += lowbit(i);
    }
}

void moveLeft() {
    for (int i = 1;i <= ai;i++) {
        addTree[i] = node[addTree[i]].left;
    }
    for (int i = 1;i <= mi;i++) {
        minusTree[i] = node[minusTree[i]].left;
    }
}

void moveRight() {
    for (int i = 1;i <= ai;i++) {
        addTree[i] = node[addTree[i]].right;
    }
    for (int i = 1;i <= mi;i++) {
        minusTree[i] = node[minusTree[i]].right;
    }
}

int Count() {
    int ans = 0;
    for (int i = 1;i <= ai;i++) {
        ans += node[addTree[i]].cnt;
    }
    for (int i = 1;i <= mi;i++) {
        ans -= node[minusTree[i]].cnt;
    }
    return ans;
}

int querySum(int jobv, int l, int r) {
    if (jobv <= 1)return 0;
    if (l == r) {
        // jobv可能不在[l, r]的值域内
        if (l < jobv) {
            return Count();
        }
        else {
            return 0;
        }
    }
    else {
        int ans = 0;
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            moveLeft();
            ans += querySum(jobv, l, mid);
        }
        else {
            // 左儿子直接计算
            // 务必维持0节点的数值为空
            for (int i = 1;i <= ai;i++) {
                ans += node[node[addTree[i]].left].cnt;
            }
            for (int i = 1;i <= mi;i++) {
                ans -= node[node[minusTree[i]].left].cnt;
            }
            moveRight();
            ans += querySum(jobv, mid + 1, r);
        }
        return ans;
    }
}

// 查询区间[jobl, jobr]严格小于jobv的元素个数
int small(int jobl, int jobr, int jobv) {
    ai = 0, mi = 0;
    int i = jobr;
    while (i > 0) {
        addTree[++ai] = innerRoot[i];
        i -= lowbit(i);
    }
    i = jobl - 1;
    while (i > 0) {
        minusTree[++mi] = innerRoot[i];
        i -= lowbit(i);
    }
    int ans = querySum(id(jobv), 1, vi);
    return ans;
}

int search(int jobk, int l, int r) {
    if (l == r) {
        return l;
    }
    int lsiz = 0;
    int mid = (l + r) >> 1;
    for (int i = 1;i <= ai;i++) {
        lsiz += node[node[addTree[i]].left].cnt;
    }
    for (int i = 1;i <= mi;i++) {
        lsiz -= node[node[minusTree[i]].left].cnt;
    }
    if (lsiz >= jobk) {
        moveLeft();
        return search(jobk, l, mid);
    }
    else {
        moveRight();
        return search(jobk - lsiz, mid + 1, r);
    }
}

// 查询区间[jobl, jobr]排名k的元素
int findRank(int jobl, int jobr, int k) {
    ai = 0, mi = 0;
    int i = jobr;
    while (i > 0) {
        addTree[++ai] = innerRoot[i];
        i -= lowbit(i);
    }
    i = jobl - 1;
    while (i > 0) {
        minusTree[++mi] = innerRoot[i];
        i -= lowbit(i);
    }
    return search(k, 1, vi);
}

// 统计区间[jobl, jobr]值x的出现次数
int CountNum(int jobl, int jobr, int x) {
    return small(jobl, jobr, x + 1) - small(jobl, jobr, x);
}

void prepare() {
    int k = vi;
    sort(val + 1, val + vi + 1);
    val[0] = -1e9;
    vi = 0;
    for (int i = 1;i <= k;i++) {
        if (val[vi] != val[i]) {
            val[++vi] = val[i];
        }
    }
}

void solve() {
    cin >> n >> m;
    space = vi = 0;
    for (int i = 1;i <= n;i++) {
        cin >> arr[i];
        val[++vi] = arr[i];
    }
    int op, l, r, k, pos;
    for (int i = 1;i <= m;i++) {
        cin >> op;
        if (op == 1) {
            cin >> l >> r >> k;
            que[i][0] = op;
            que[i][1] = l;
            que[i][2] = r;
            que[i][3] = k;
        }
        else if (op == 2) {
            cin >> l >> r >> k;
            que[i][0] = op;
            que[i][1] = l;
            que[i][2] = r;
            que[i][3] = k;
        }
        else if (op == 3) {
            cin >> pos >> k;
            que[i][0] = op;
            que[i][1] = pos;
            que[i][2] = k;
        }
        else if (op == 4) {
            cin >> l >> r >> k;
            que[i][0] = op;
            que[i][1] = l;
            que[i][2] = r;
            que[i][3] = k;
        }
        else {
            cin >> l >> r >> k;
            que[i][0] = op;
            que[i][1] = l;
            que[i][2] = r;
            que[i][3] = k;
        }
        val[++vi] = k;
    }
    prepare();
    for (int i = 1;i <= n;i++) {
        add(i, arr[i], 1);
    }
    for (int i = 1;i <= m;i++) {
        op = que[i][0];
        if (op == 1) {
            l = que[i][1];
            r = que[i][2];
            k = que[i][3];
            cout << small(l, r, k) + 1 << '\n';
        }
        else if (op == 2) {
            l = que[i][1];
            r = que[i][2];
            k = que[i][3];
            cout << val[findRank(l, r, k)] << '\n';
        }
        else if (op == 3) {
            pos = que[i][1];
            k = que[i][2];
            add(pos, arr[pos], -1);
            add(pos, k, 1);
            arr[pos] = k;
        }
        else if (op == 4) {
            l = que[i][1];
            r = que[i][2];
            k = que[i][3];
            int kth = small(l, r, k);
            if (kth == 0) {
                cout << -2147483647 << '\n';
            }
            else {
                cout << val[findRank(l, r, kth)] << '\n';
            }
        }
        else {
            l = que[i][1];
            r = que[i][2];
            k = que[i][3];
            int kth = small(l, r, k) + CountNum(l, r, k);
            if (kth == r - l + 1) {
                cout << 2147483647 << '\n';
            }
            else {
                cout << val[findRank(l, r, kth + 1)] << '\n';
            }
        }
    }
}
