#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
const int MAXT = MAXN * 300;
using ll = long long;

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

int val[MAXN + 1];
int vi = 0;

int addTree[MAXN + 1];
int ai = 0;

int arr[MAXN + 1];

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
}

void moveRight() {
    for (int i = 1;i <= ai;i++) {
        addTree[i] = node[addTree[i]].right;
    }
}

int Count() {
    int ans = 0;
    for (int i = 1;i <= ai;i++) {
        ans += node[addTree[i]].cnt;
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
            moveRight();
            ans += querySum(jobv, mid + 1, r);
        }
        return ans;
    }
}

// 查询区间[jobl, jobr]严格小于jobv的元素个数
int small(int jobr, int jobv) {
    if (jobr < 1)return 0;
    ai = 0;
    int i = jobr;
    while (i > 0) {
        addTree[++ai] = innerRoot[i];
        i -= lowbit(i);
    }
    int ans = querySum(id(jobv), 1, vi);
    return ans;
}

// 统计区间[jobl, jobr]值x的出现次数
int CountNum(int jobr, int x) {
    return small(jobr, x + 1) - small(jobr, x);
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

// 查询区间[jobl, jobr]严格大于jobv的元素个数
ll moreNum(int jobr, int jobv) {
    if (jobr < 1)return 0;
    ll ans = small(jobr, jobv + 1);
    return jobr - ans;
}

void solve() {
    cin >> n;
    space = vi = 0;
    for (int i = 1;i <= n;i++) {
        cin >> arr[i];
        val[++vi] = arr[i];
    }
    prepare();
    for (int i = 1;i <= n;i++) {
        add(i, arr[i], 1);
    }
    ll ans = 0;
    for (int i = 1;i <= n;i++) {
        ans += moreNum(i - 1, arr[i]);
    }
    cout << ans << '\n';
    cin >> m;
    int a, b;
    while (m--) {
        cin >> a >> b;
        if (a > b)swap(a, b);
        // 去除a, b原先的逆序对

        // arr[a], arr[b]的关系最后处理
        add(a, arr[a], -1);
        add(b, arr[b], -1);

        ans += (moreNum(b, arr[a]) - moreNum(a, arr[a]));
        ans -= (small(b, arr[a]) - small(a, arr[a]));

        ans += (small(b, arr[b]) - small(a, arr[b]));
        ans -= (moreNum(b, arr[b]) - moreNum(a, arr[b]));

        if (arr[a] < arr[b]) {
            ans++;
        }
        else if (arr[a] > arr[b]) {
            ans--;
        }

        swap(arr[a], arr[b]);
        add(a, arr[a], 1);
        add(b, arr[b], 1);

        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
