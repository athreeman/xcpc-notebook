#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// Kummer定理:
// vp(x): x质因子中p的质数(幂)
// sp(x): x在p进制下的各位之和

// 用于判断
// 1.组合数或多重集能否没p的幂整除
// 2.mod p 是否=0

// 二项式:
// k = vp(C(n, m)) = (sp(m) + sp(n - m) - sp(n)) / (p - 1) = [m + (n - m)在p进制下相加的进位次数]
// p的幂次(即该组合数结果中p因子的出现次数)

// 多重集: m是一个大小为r集合
// k = vp(C(n, (m[0], m[1], m[2],..., m[r - 1]))) = (sp(m[0]) +... + sp(m[r - 1]) - sp(n)) / (p - 1) = m[0] + m[1] +...+ m[r - 1]
// p的幂次(即该组合数结果中p因子的出现次数)

// k = 0, 不被p整除
// k >= 1, 被p整数(且最大被p的k次幂整除)


//=====当p=2时======//

// 二项式:
// C(n, m)为奇数 ⟺ m & (n - m) == 0 ⟺ (m & n) == m

// 多重集: (包含r个元素的集合m)
// n! / (m[0]! * m[1]! *...* m[r-1]!)为奇数
// 等价于: 各 m[i] 的二进制 1 位两两不相交, 且全体并集恰好是 n 的全部 1 位
// 集合m的元素累加和 = n且每一位上的1只出现一次, 即 n 的每个 1 位, 恰好分给唯一一个 m[i]

// sp(x, p): 各位数字之和
ll sp(ll x, ll p) {
    ll s = 0;
    while (x) {
        s += x % p;
        x /= p;
    }
    return s;
}

// 二项式:
ll vp_binom(ll n, ll m, ll p) {
    return (sp(m, p) + sp(n - m, p) - sp(n, p)) / (p - 1);
}

// 多重集:
ll vp_multiset(const ll m[], ll r, ll n, ll p) {
    ll s = 0;
    for (ll i = 0; i < r; i++) s += sp(m[i], p);
    return (s - sp(n, p)) / (p - 1);
}

// 判断 C(n, m) mod p == 0
bool div_by_p(ll n, ll m, ll p) {
    return vp_binom(n, m, p) >= 1;
}

