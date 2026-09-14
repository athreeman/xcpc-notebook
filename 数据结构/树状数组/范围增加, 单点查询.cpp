#include<bits/stdc++.h>
using namespace std;
//树状数组,树状数组的下标必须以1为基底
//功能:主要用于维护可差分信息，如累加和、乘积...

//一维树状数组：范围增加+单点查询
//范围增加依赖于维护一维差分数组

class Range_AND_Single {
    static const int MAXN = 2e5;
    static int treeDiff[MAXN + 1];

    int lowbit(int x) {
        return x & -x;
    }
    //单点增加,基于差分数组
    void addSingle(int n, int i, int val) {
        while (i <= n) {//n是右边界
            treeDiff[i] += val;
            i += lowbit(i);
        }
    }
    //范围增加
    void addRange(int n, int l, int r, int val) {
        addSingle(n, l, val);
        addSingle(n, r + 1, -val);
    }
    //单点查询
    int querySingle(int i) {
        int ans = 0;
        while (i > 0) {
            ans += treeDiff[i];
            i -= lowbit(i);
        }
        return ans;
    }
};