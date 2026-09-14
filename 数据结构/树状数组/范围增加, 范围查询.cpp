#include<bits/stdc++.h>
using namespace std;
//树状数组,树状数组的下标必须以1为基底
//功能:主要用于维护可差分信息，如累加和、乘积...


//一维树状数组：范围增加+范围查询
//依赖两个一维数组维护
class Range_AND_Range {
    static const int MAXN = 2e5;
    static int tree1[MAXN + 1];//一维差分数组
    static int tree2[MAXN + 1];//tree2[i]=(i-1)*d[i],辅助数组
    int lowbit(int x) {
        return x & -x;
    }
    //单点增加
    void add(int n, int* tree, int i, int val) {
        while (i <= n) {
            tree[i] += val;
            i += lowbit(i);
        }
    }
    //树tree区间和[1...i]
    int sum(int* tree, int i) {
        //1...i
        int ans = 0;
        while (i > 0) {
            ans += tree[i];
            i -= lowbit(i);
        }
        return ans;
    }
    //tree区间修改
    void addRange(int n, int l, int r, int v) {
        add(n, tree1, l, v);
        add(n, tree1, r + 1, -v);
        add(n, tree2, l, (l - 1) * v);
        add(n, tree2, r + 1, -r * v);
    }
    //求原数组区间和
    int queryRange(int l, int r) {
        return sum(tree1, r) * r - sum(tree2, r) - sum(tree1, l - 1) * (l - 1) + sum(tree2, l - 1);
    }
};
