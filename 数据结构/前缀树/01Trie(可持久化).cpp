#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
const int MAXT = (MAXN + MAXN + 2) * 26;

// 可持久化前缀树
// 常被用于区间异或值等问题
// https://www.luogu.com.cn/problem/P4735

int bit = 25;
int version[MAXN << 1 | 1];
int tree[MAXT][2];
int pass[MAXT];
int end[MAXT];
int space = 0;

int clone(int i) {
    space++;
    tree[space][0] = tree[i][0];
    tree[space][1] = tree[i][1];
    pass[space] = pass[i];
    ::end[space] = ::end[i];
    return space;
}

int insert(int val, int i) {
    int rt = clone(i);
    pass[rt]++;
    int pre = rt, cur, path;
    for (int b = bit;b >= 0;b--, pre = cur) {// 每次结束更新pre
        path = (val >> b) & 1;// 路径
        i = tree[i][path];// 下一个节点
        cur = clone(i);// 拷贝
        pass[cur]++;// pass累加
        tree[pre][path] = cur;// 更新pre的路径信息
    }
    ::end[cur]++;
    return rt;// 返回新的头节点
}

// 查询值val, 和区间[l, r]的非空子序列异或可以得到的最大异或和
// u是l - 1的树头, v是r的树头
int query(int val, int u, int v) {
    int ans = 0;
    int path, best;
    for (int b = bit;b >= 0;b--) {
        path = (val >> b) & 1;
        best = path ^ 1;
        if (pass[tree[v][best]] > pass[tree[u][best]]) {
            v = tree[v][best];
            u = tree[u][best];
            ans += (1 << b);
        }
        else {
            v = tree[v][path];
            u = tree[u][path];
        }
    }
    return ans;
}

void clear() {
    version[0] = 0;
    tree[0][0] = tree[0][1] = 0;
    pass[0] = ::end[0] = 0;
}

void solve() {
    int n, m;
    cin >> n >> m;
    int Xor = 0;
    int cnt = 0;
    clear();// 0节点清空
    cnt++;// 补0
    version[cnt] = insert(0, version[cnt - 1]);
    for (int i = 1;i <= n;i++) {
        int in;
        cin >> in;
        Xor ^= in;
        cnt++;
        version[cnt] = insert(Xor, version[cnt - 1]);
    }
    while (m--) {
        char op;
        cin >> op;
        int x, l, r;
        if (op == 'A') {
            cin >> x;
            Xor ^= x;
            cnt++;
            version[cnt] = insert(Xor, version[cnt - 1]);
        }
        else {
            cin >> l >> r >> x;
            x ^= Xor;
            cout << query(x, version[l - 1], version[r]) << '\n';
        }
    }
}
