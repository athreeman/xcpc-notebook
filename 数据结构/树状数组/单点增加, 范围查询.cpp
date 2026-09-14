#include<bits/stdc++.h>
using namespace std;
//树状数组,树状数组的下标必须以1为基底
//功能:主要用于维护可差分信息，如累加和、乘积...

class Single_AND_Range {
private:
    static const int MAXN = 2e5;
    static int tree[MAXN + 1];
    //获取最低位1
    int lowbit(int x) {
        return x & -x;
    }
    //单点增加
    void addSingle(int n, int i, int val) {
        while (i <= n) {//n是右边界
            tree[i] += val;
            i += lowbit(i);
        }
    }

    //普通求和[1...i]
    int sum(int i) {
        int ans = 0;
        while (i > 0) {
            ans += tree[i];
            i -= lowbit(i);
        }
        return ans;
    }
    //区间查询
    int queryRange(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};