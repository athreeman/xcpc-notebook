#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// FHQTreap
// 核心: 根据key值执行分裂, 根据优先级执行合并操作

// merge(l,r)：合并后树的中序遍历顺序就是先l树,再r树
// 因此,FHQ树可以巧妙地维护中序遍历的特定顺序
// 同时又因为优先级的设定, 使得树高也得到维护

// 如若只在乎特定的顺序, merge和split的结合更为灵活, 此时所谓的排名rank就是中序遍历顺序
// 形象地说, 如果把中序遍历结果放进一维数组, 此时的rank就是下标, 即维护的就是下标

// 如若在乎key值的大小关系, 那么merge操作对于split分裂后的左右子树, 顺序不能改变, rank不仅表示下标, 还表示第k小
// 否则rank失去了关于元素大小的比较功能, 只能代表遍历顺序, 即只维护下标

const int MAXN = 2e5;
const ll INF = 1e18;
int head = 0, cnt = 0;
struct {
    ll key;
    int priority;
    int left;
    int right;
    int size;// 节点总数
}node[MAXN + 1];

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int>dist(1, INT_MAX);

void clear() {
    for (int i = 0;i <= cnt;i++) {
        node[i] = { 0,0,0,0,0 };
    }
    head = cnt = 0;
}

void up(int i) {
    node[i].size = node[node[i].left].size + node[node[i].right].size + 1;
}

// 若当前 curv <= key, 则左侧子树不需要改变, 向右子树递归
// 反之, 右侧子树不需要改变, 向左子树递归

// key值分裂 (改为地址传递)
void splitByKey(int i, ll key, int& x, int& y) {
    if (i == 0) {
        x = y = 0;
    }
    else {
        if (node[i].key <= key) {
            x = i;
            splitByKey(node[i].right, key, node[i].right, y);
        }
        else {
            y = i;
            splitByKey(node[i].left, key, x, node[i].left);
        }
        up(i);
    }
}

// 经典非词频压缩, 可正常使用 (改为地址传递)
void splitByRank(int i, ll rk, int& x, int& y) {
    if (i == 0) {
        x = y = 0;
    }
    else {
        if (node[node[i].left].size + 1 <= rk) {
            x = i;
            splitByRank(node[i].right, rk - node[node[i].left].size - 1, node[i].right, y);
        }
        else {
            y = i;
            splitByRank(node[i].left, rk, x, node[i].left);
        }
        up(i);
    }
}

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

void add(ll val) {
    int x, y;
    splitByKey(head, val, x, y);
    node[++cnt].key = val;
    node[cnt].size = 1;
    node[cnt].priority = dist(gen);
    head = merge(merge(x, cnt), y);
}

void remove(ll val) {
    int x, y;
    splitByKey(head, val, x, y);      // x: <=val, y: >val
    int l, m;
    splitByKey(x, val - 1, l, m);     // l: <=val-1, m: =val
    // =val的去掉一个(头), val-1部分和>val部分全部保留
    head = merge(merge(l, merge(node[m].left, node[m].right)), y);
}

int small(ll val) {
    int x, y;
    splitByKey(head, val - 1, x, y);
    int ans = node[x].size;
    head = merge(x, y);
    return ans;
}

int rank(ll val) {
    return small(val) + 1;
}

ll index(int i, int x) {
    if (node[node[i].left].size >= x) {
        return index(node[i].left, x);
    }
    else if (node[node[i].left].size + 1 < x) {
        return index(node[i].right, x - node[node[i].left].size - 1);
    }
    return node[i].key;
}

ll index(int x) {
    return index(head, x);
}

ll pre(int i, ll val) {
    if (i == 0) {
        return -INF;
    }
    if (node[i].key >= val) {
        return pre(node[i].left, val);
    }
    else {
        return max(node[i].key, pre(node[i].right, val));
    }
}

ll pre(ll val) {
    return pre(head, val);
}

ll post(int i, ll val) {
    if (i == 0) {
        return INF;
    }
    if (node[i].key <= val) {
        return post(node[i].right, val);
    }
    else {
        return min(node[i].key, post(node[i].left, val));
    }
}

ll post(ll val) {
    return post(head, val);
}
