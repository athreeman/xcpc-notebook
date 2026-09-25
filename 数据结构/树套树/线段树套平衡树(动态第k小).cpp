#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// https://www.luogu.com.cn/problem/P3380

// 解题思路：
// 线段树套fhq树(经典非词频压缩)
// 操作1，外层找到位置后进入内层，分别查询严格小于k的元素个数，然后累加和 + 1
// 操作2，对答案二分查找
// 操作3，外层找到叶节点，确定要修改的值是多少，回溯时候进入内层逐一修改
// 操作4，找到外层后，内层逐一查找，取最大值
// 操作5，同上，不过最后取最小值

const int MAXN = 2e5;
const int MAXT = 8000000;

// 外层线段树
int innerRoot[MAXN << 2 | 1];
int arr[MAXN + 1];

int n, m;
struct FHQ {
    int key;
    int priority;
    int left;
    int right;
    int size;
}node[MAXT + 1];
int space = 0;

random_device rd;
mt19937 gen(rd());

void up(int i) {
    node[i].size = node[node[i].left].size + node[node[i].right].size + 1;
}

int create(int val) {
    space++;
    node[space].key = val;
    node[space].left = node[space].right = 0;
    node[space].priority = gen();// 注意！：该题此处舍弃dist(gen)的写法，直接使用gen()，否则TLE
    node[space].size = 1;
    return space;
}

// 根据key分裂i这棵树
void split(int& l, int& r, int i, ll key) {
    if (i == 0) {
        l = r = 0;
    }
    else {
        if (node[i].key <= key) {
            l = i;
            split(node[i].right, r, node[i].right, key);
        }
        else {
            r = i;
            split(l, node[i].left, node[i].left, key);
        }
        up(i);
    }
}

// 根据优先级合并l和r这两棵树
int merge(int l, int r) {
    if (l == 0 || r == 0) {
        return l + r;
    }
    if (node[l].priority >= node[r].priority) {
        node[l].right = merge(node[l].right, r);
        up(l);
        return l;
    }
    else {
        node[r].left = merge(l, node[r].left);
        up(r);
        return r;
    }
}

// 查询严格小于val的元素个数
int small(int i, int val) {
    int l, r;
    split(l, r, innerRoot[i], val - 1);
    int ans = node[l].size;
    innerRoot[i] = merge(l, r);
    return ans;
}

// 树内的一个val改为to
void update(int i, int val, int to) {
    int l, r;
    split(l, r, innerRoot[i], val);

    int lm, rm;
    split(lm, rm, l, val - 1);

    innerRoot[i] = merge(merge(lm, merge(node[rm].left, node[rm].right)), r);

    split(l, r, innerRoot[i], to);

    innerRoot[i] = merge(merge(l, create(to)), r);
}

void add(int i, int val) {
    int l, r;
    split(l, r, innerRoot[i], val);
    innerRoot[i] = merge(merge(l, create(val)), r);
}

// 查询val的前驱
int pre(int head, int val) {
    if (head == 0) {
        return -2147483647;
    }
    if (node[head].key >= val) {
        return pre(node[head].left, val);
    }
    else {
        return max(node[head].key, pre(node[head].right, val));
    }
}

// 查询val的后继
int post(int head, int val) {
    if (head == 0) {
        return 2147483647;
    }
    if (node[head].key <= val) {
        return post(node[head].right, val);
    }
    else {
        return min(node[head].key, post(node[head].left, val));
    }
}

// 在区间[l, r]严格小于k的元素个数
int small(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return small(i, jobv);
    }
    else {
        int mid = (l + r) >> 1;
        int ans = 0;
        if (jobl <= mid) {
            ans += small(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            ans += small(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        return ans;
    }
}

// jobi位置的值jobv更改为to
void update(int jobi, int jobv, int to, int l, int r, int i) {
    if (l != r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            update(jobi, jobv, to, l, mid, i << 1);
        }
        else {
            update(jobi, jobv, to, mid + 1, r, i << 1 | 1);
        }
    }
    update(i, jobv, to);
}

// 查询[jobl, jobr]范围内val的前驱
int pre(int jobl, int jobr, int val, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return pre(innerRoot[i], val);
    }
    else {
        int mid = (l + r) >> 1;
        int ans = -2147483647;
        if (jobl <= mid) {
            ans = max(ans, pre(jobl, jobr, val, l, mid, i << 1));
        }
        if (jobr > mid) {
            ans = max(ans, pre(jobl, jobr, val, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

int post(int jobl, int jobr, int val, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return post(innerRoot[i], val);
    }
    else {
        int mid = (l + r) >> 1;
        int ans = 2147483647;
        if (jobl <= mid) {
            ans = min(ans, post(jobl, jobr, val, l, mid, i << 1));
        }
        if (jobr > mid) {
            ans = min(ans, post(jobl, jobr, val, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

// 查询区间排名第k的数
int findRank(int jobl, int jobr, int k) {
    int l = 0, r = 1e8;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (small(jobl, jobr, mid, 1, n, 1) + 1 > k) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    return l - 1;
}

void insert(int jobi, int jobv, int l, int r, int i) {
    if (l != r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            insert(jobi, jobv, l, mid, i << 1);
        }
        else {
            insert(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
    }
    add(i, jobv);
}

void solve() {
    cin >> n >> m;
    for (int i = 1;i <= n;i++) {
        cin >> arr[i];
        insert(i, arr[i], 1, n, 1);
    }
    while (m--) {
        int op, pos, l, r, k;
        cin >> op;
        if (op == 1) {
            cin >> l >> r >> k;
            cout << small(l, r, k, 1, n, 1) + 1 << '\n';
        }
        else if (op == 2) {
            cin >> l >> r >> k;
            cout << findRank(l, r, k) << '\n';
        }
        else if (op == 3) {
            cin >> pos >> k;
            update(pos, arr[pos], k, 1, n, 1);
            arr[pos] = k;
        }
        else if (op == 4) {
            cin >> l >> r >> k;
            cout << pre(l, r, k, 1, n, 1) << '\n';
        }
        else {
            cin >> l >> r >> k;
            cout << post(l, r, k, 1, n, 1) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
