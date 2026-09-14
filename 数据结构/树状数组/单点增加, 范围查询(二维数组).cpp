#include<bits/stdc++.h>
using namespace std;
//树状数组,树状数组的下标必须以1为基底
//功能:主要用于维护可差分信息，如累加和、乘积...

// 二维依赖矩阵求和
class SingleAndRange {
    //单点增加+范围查询
    static const int MAXN = 2000;
    static int tree[MAXN + 1][MAXN + 1];

    int lowbit(int x) {
        return x & -x;
    }
    //(1,1)->(x,y)范围增加
    void add(int x, int y, int v, int n, int m) {
        for (int i = x;i <= n;i += lowbit(i)) {
            for (int j = y;j <= m;j += lowbit(j)) {
                tree[i][j] += v;
            }
        }
    }
    //(1,1)->(x,y)范围求和
    int sum(int x, int y) {
        int ans = 0;
        for (int i = x;i > 0;i -= lowbit(i)) {
            for (int j = y;j > 0;j -= lowbit(j)) {
                ans += tree[i][j];
            }
        }
        return ans;
    }
    //范围求和(l,r)->(x,y)
    int queryRange(int l, int r, int x, int y) {
        return sum(x, y) - sum(x, r - 1) - sum(l - 1, y) + sum(l - 1, r - 1);
    }
};
