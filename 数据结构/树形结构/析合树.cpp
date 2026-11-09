#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
    定义一个排列p, 包含1~n的每个元素刚好出现一次
    p的一个区间[l, r], 排序后相邻元素的差 = 1, 称为一个连续段
    连续段满足 mx - mn = r - l

    连续段的性质:

    定义连续段A: [a, b] B : [l, r]
    若连续段A, 不存在任何连续段B满足a < l ≤ b < r 或 l < a ≤ r < b, 则称A为本原连续段, 本原连续段之间要么包含, 要么不相交
    若满足 a <= l <= b <= r
    则他们的交集[l, b], 并集[a, r], 差集[a, l - 1], [b + 1, r]也都是连续段

    定义:
    节点: 每一个本原连续段都作为一个节点
    根据包含关系, 去构建父子关系(必须是直接包含关系, 即不能存在A包含C, C包含B的嵌套包含)
    若A[a, b], B[l, r]满足a < l <= r < b且直接包含, 则B是A的儿子

    节点信息: 该节点对应连续段在排列p上的 下标区间 和 值域区间

    儿子序列: 一个节点的每个儿子节点按照原数组顺序(即下标区间)依次构成的序列
    儿子排列: 一个节点的儿子序列依据值域左端升序赋予排名后, 形成的序列

    合点: 一个节点的儿子排列呈现递增或递减(不存在儿子的也视为合点)
    析点: 如果一个节点不是合点, 那他就是析点

    性质:
    合点的任意连续儿子组合, 都是连续段
    析点的任意真连续儿子组合都不是连续段(除了单一儿子和整体)

    建树过程:
    我们用一个栈, 存储信息: {下标区间, 值域区间, 方向(即单调性)}, 初始为空
    定义操作:
    新增: 新增一个下标和值域区间长度 = 1的节点, 视为合点
    合并: 从栈顶找到最短的一段连续的节点, 使得和当前节点合并后成为一个连续段
    并入: 若满足 栈顶是合点, 当前节点的下标和值域区间可以和栈顶的衔接, 当前节点合并后不破坏单调性
        则并入栈顶结点(成为儿子), 并扩展栈顶节点的下标和值域区间

    具体流程:
    新建叶子节点
    栈空: 新增
    栈非空:
    while (1):
        可并入 -> 并入, continue
        可合并 -> 合并, continue
        否则 break
    压栈

    注: 只有当并入操作变得非法时, 才能检查合并操作, 并入时如果被并点是叶节点, 需要额外拷贝一份

    其中, 检查是否是合点的过程, 只出现在合并过程
    若单次合并节点数(包括当前节点) > 2则一定是析点, 恰好2个合点
    如果 >= 3个节点还是单调, 最后两个节点会在更早时刻并入
*/

const int MAXN = 3e5;

int n;
int p[MAXN + 1];

// 建图, c采用邻接表建图, 避免链式前向星儿子方向错误的问题
vector<int>e[2 * MAXN + 1];

// 树结构
int root;
int space = 0;
struct Tree {
    int pl, pr;// 下标区间
    int mn, mx;// 值域区间
    int type;// 0:任意方向 1:合点 -1:析点
    int dir;// 0:递减 1:递增
}node[2 * MAXN + 1];

// 手写栈
struct Stack {

    int data[MAXN + 1];

    int si = 0;
    void clear() { si = 0; }
    void push(int i) { data[++si] = i; }
    int top() { return data[si]; }
    int pop() { return data[si--]; }
    bool empty() { return si == 0; }
    int size() { return si; }

}sta, staMax, staMin, tmp;

void addEdge(int u, int v) {
    e[u].push_back(v);
}

int create(int i) {
    space++;
    node[space].pl = node[space].pr = i;
    node[space].mn = node[space].mx = p[i];
    node[space].dir = node[space].type = 0;
    return space;
}

// 线段树, 辅助实现合并查询
struct SegTree {
    int mx, mn, l;// min来自哪个mx,mn,l
    int min;// mx - mn + l的最小值
    bool isUpdateMax, isUpdateMin;
    int mxTo, mnTo;
}info[MAXN << 2 | 1];

void lazy(int i, bool isMx, int to) {
    if (isMx) {
        info[i].min += (to - info[i].mx);
        info[i].mx = to;
        info[i].mxTo = to;
        info[i].isUpdateMax = true;
    }
    else {
        info[i].min -= (to - info[i].mn);
        info[i].mn = to;
        info[i].mnTo = to;
        info[i].isUpdateMin = true;
    }
}

void down(int i) {
    if (info[i].isUpdateMax) {
        lazy(i << 1, true, info[i].mxTo);
        lazy(i << 1 | 1, true, info[i].mxTo);
        info[i].isUpdateMax = false;
    }
    if (info[i].isUpdateMin) {
        lazy(i << 1, false, info[i].mnTo);
        lazy(i << 1 | 1, false, info[i].mnTo);
        info[i].isUpdateMin = false;
    }
}

void up(int i) {
    if (info[i << 1].min < info[i << 1 | 1].min) {
        info[i].min = info[i << 1].min;
        info[i].mn = info[i << 1].mn;
        info[i].mx = info[i << 1].mx;
        info[i].l = info[i << 1].l;
    }
    else {
        info[i].min = info[i << 1 | 1].min;
        info[i].mn = info[i << 1 | 1].mn;
        info[i].mx = info[i << 1 | 1].mx;
        info[i].l = info[i << 1 | 1].l;
    }
}

void initTree(int l, int r, int i) {
    info[i].min = 1e9;
    info[i].isUpdateMax = false;
    info[i].isUpdateMin = false;
    if (l != r) {
        int mid = (l + r) >> 1;
        initTree(l, mid, i << 1);
        initTree(mid + 1, r, i << 1 | 1);
        up(i);
    }
    else {
        info[i].l = l;
        info[i].min = l;
        info[i].mx = p[l];
        info[i].mn = p[l];
    }
}

// 区间[jobl, jobr]的mx 或 mn进行更新
void update(int jobl, int jobr, bool isMx, int to, int l, int r, int i) {
    if (jobl > jobr)return;
    if (jobl <= l && r <= jobr) {
        lazy(i, isMx, to);
    }
    else {
        down(i);
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            update(jobl, jobr, isMx, to, l, mid, i << 1);
        }
        if (jobr > mid) {
            update(jobl, jobr, isMx, to, mid + 1, r, i << 1 | 1);
        }
        up(i);
    }
}


int find(int tar, int R, int l, int r, int i) {
    if (l == r) {
        return (l <= R && info[i].min == tar) ? info[i].l : 0;;
    }
    else {
        down(i);
        int mid = (l + r) >> 1;
        int ans = 0;
        if (mid + 1 <= R && info[i << 1 | 1].min == tar) {
            ans = find(tar, R, mid + 1, r, i << 1 | 1);
        }
        if (!ans && info[i << 1].min == tar) {
            ans = find(tar, R, l, mid, i << 1);
        }
        up(i);
        return ans;
    }
}

int merge(int u, int v) {
    int i = create(1);
    node[i].pl = node[u].pl;
    node[i].pr = node[v].pr;
    node[i].mn = min(node[u].mn, node[v].mn);
    node[i].mx = max(node[u].mx, node[v].mx);
    addEdge(i, u);
    addEdge(i, v);
    return i;
}

void buildTree() {
    p[0] = 0;
    root = 0;
    space = 0;
    initTree(1, n, 1);
    for (int i = 1;i <= 2 * n - 1;i++) {
        e[i].clear();
    }
    for (int i = 1; i <= n; i++) {

        // 单调栈, 维护最大值
        while (staMax.size() && p[i] > p[staMax.top()]) {
            int j = staMax.pop();
            int L = staMax.empty() ? 1 : staMax.top() + 1;
            update(L, j, true, p[i], 1, n, 1);
        }
        staMax.push(i);

        // 单调栈, 维护最小值
        while (staMin.size() && p[i] < p[staMin.top()]) {
            int j = staMin.pop();
            int L = staMin.empty() ? 1 : staMin.top() + 1;
            update(L, j, false, p[i], 1, n, 1);
        }
        staMin.push(i);

        // 新叶节点
        int cur = create(i);

        while (sta.size()) {

            int u = sta.top();// 栈顶
            bool ok = false;

            if (node[u].type == 0) {// 栈顶是叶节点
                int d = -1;
                // 判断方向及合法性
                if (node[u].mx + 1 == node[cur].mn) d = 1, ok = true;
                else if (node[u].mn - 1 == node[cur].mx) d = 0, ok = true;

                // 合并
                if (ok) {
                    cur = merge(u, cur);
                    node[cur].type = 1;
                    node[cur].dir = d;
                    sta.pop();
                    continue;
                }
            }
            else if (node[u].type == 1) {// 栈顶是合点
                // 判断方向及合法性
                if (node[u].dir == 1 && node[u].mx + 1 == node[cur].mn) ok = true;
                if (node[u].dir == 0 && node[u].mn - 1 == node[cur].mx) ok = true;
                // 合并, cur成为u的儿子, 扩展u的区间大小
                if (ok) {
                    node[u].pr = node[cur].pr;
                    node[u].mn = min(node[u].mn, node[cur].mn);
                    node[u].mx = max(node[u].mx, node[cur].mx);
                    addEdge(u, cur);
                    cur = u;
                    sta.pop();
                    continue;
                }
            }

            // 查找mx - mn + l = i的最右l
            int pos = find(i, node[cur].pl - 1, 1, n, 1);
            if (pos != 0) {// 找到了
                tmp.clear();
                // 查找需要合并的点
                while (sta.size() && node[sta.top()].pl >= pos) {
                    tmp.push(sta.pop());
                }
                if (tmp.size() == 0) break;

                int cnt = 0;
                // 需要合并的点, 全部成为新节点的儿子, 并逐步扩大新节点的区间
                int w = create(0);
                node[w].pl = pos;
                node[w].pr = i;
                node[w].mn = node[cur].mn;
                node[w].mx = node[cur].mx;

                while (tmp.size()) {
                    int v = tmp.pop();
                    addEdge(w, v);
                    node[w].mn = min(node[w].mn, node[v].mn);
                    node[w].mx = max(node[w].mx, node[v].mx);
                    cnt++;
                }
                addEdge(w, cur);

                int sons = cnt + 1;
                node[w].type = (sons == 2 ? 1 : -1);
                if (sons == 2)
                    node[w].dir = (node[w].mn == node[cur].mn ? 0 : 1);

                cur = w;
                continue;
            }
            break;
        }

        sta.push(cur);
        root = cur;
    }
    sta.clear();
    staMin.clear();
    staMax.clear();
}

// 模板例题: https://codeforces.com/problemset/problem/526/F
// 给定排列, 长度n, 求连续段个数

ll ans;
void dfs(int u) {
    if (e[u].size() == 0 || node[u].type == -1) {
        ans++;
    }
    if (node[u].type == 1) {
        ll k = e[u].size();
        ans = ans + k * (k - 1) / 2LL;
    }
    for (int v : e[u]) {
        dfs(v);
    }
}

void solve() {
    cin >> n;
    for (int i = 1;i <= n;i++) {
        int x, y;
        cin >> x >> y;
        p[x] = y;
    }
    buildTree();
    ans = 0;
    dfs(root);
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}