#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll mod = 998244353;

// 根据数值的全排列
// n: 多重集合的元素个数
// a[k]: 元素k的出现次数
// S: 全排列个数
// vi: 根据升序排序后的某种元素(数值)
// S = n! / a[v1]! * a[v2]! * a[v3]! * ... * a[vn]!


// 根据下标的全排列(即每个元素不论数值都视为不同)
// n: 元素个数
// S: 全排列个数
// S = n!

ll fac[114514];// 阶乘在模mod下的值
int s[114514];// 多重集
int cnt[114514];// 词频
ll inv_(ll a) {
    // 逆元, 略...
    return 0;
};

ll f1(int n, int m) {
    ll ans = fac[n];
    for (int i = 1;i <= m;i++) {
        ans = ans * inv_(fac[cnt[s[i]]]);
    }
    return ans;
}

ll f2(int n) {
    return fac[n];
}