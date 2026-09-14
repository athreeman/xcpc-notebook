#include<bits/stdc++.h>
using namespace std;
//树状数组,树状数组的下标必须以1为基底
//功能:主要用于维护可差分信息，如累加和、乘积...

// 二维查询依赖于差分数组维护信息
class RangeAndRange {
    //范围增加+范围查询
    static const int MAXN = 2000;
    //二维差分数组,tree维护四个信息
    //diff[i][j]
    static int tree1[MAXN + 1][MAXN + 1];
    //diff[i][j]*i
    static int tree2[MAXN + 1][MAXN + 1];
    //diff[i][j]*j
    static int tree3[MAXN + 1][MAXN + 1];
    //diff[i][j]*i*j
    static int tree4[MAXN + 1][MAXN + 1];

    int lowbit(int x) {
        return x & -x;
    }

    //位置(x,y)单点增加
    void addSingle(int x, int y, int v, int n, int m) {
        int v1 = v;
        int v2 = v * x;
        int v3 = v * y;
        int v4 = x * y * v;
        for (int i = x;i <= n;i += lowbit(i)) {
            for (int j = y;j <= m;j += lowbit(j)) {
                tree1[i][j] += v1;
                tree2[i][j] += v2;
                tree3[i][j] += v3;
                tree4[i][j] += v4;
            }
        }
    }
    //求和(1,1)->(x,y)
    int sum(int x, int y) {
        int ans = 0;
        for (int i = x;i > 0;i -= lowbit(i)) {
            for (int j = y;j > 0;j -= lowbit(j)) {
                //四个信息 
                ans += (x + 1) * (y + 1) * tree1[i][j]
                    - (y + 1) * tree2[i][j]
                    - (x + 1) * tree3[i][j]
                    + tree4[i][j];
            }
        }
        return ans;
    }
    //范围增加
    void addRange(int a, int b, int c, int d, int v, int n, int m) {
        addSingle(a, b, v, n, m);
        addSingle(a, d + 1, -v, n, m);
        addSingle(c + 1, b, -v, n, m);
        addSingle(c + 1, d + 1, v, n, m);
    }
    //范围查询
    int queryRange(int a, int b, int c, int d, int n, int m) {
        int ans = 0;
        ans = sum(c, d) - sum(a - 1, d) - sum(c, b - 1) + sum(a - 1, b - 1);
        return ans;
    }

};
