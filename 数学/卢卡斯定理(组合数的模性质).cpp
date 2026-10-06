#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 给定n, m >= 1, 质数p >= 2
// 求解C(n, m) % p的值

// 静态空间：阶乘与逆元表，大小需 >= 最大质数 p
const int MAXP = 1000005;

ll fact[MAXP], inv_fact[MAXP];
// 快速幂 a^b % mod
ll qpow(ll a, ll b, ll mod) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

// 预处理 0..p-1 的阶乘与逆元（Lucas 中每一位都 < p）
void init(ll p) {
    fact[0] = 1;
    for (int i = 1; i < p; i++)
        fact[i] = fact[i - 1] * i % p;

    inv_fact[p - 1] = qpow(fact[p - 1], p - 2, p);
    for (int i = p - 2; i >= 0; i--)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % p;
}



// 小组合数：C(n, m) % p，其中 0 <= n, m < p
ll C_small(ll n, ll m, ll p) {
    if (m > n) return 0;
    return fact[n] * inv_fact[m] % p * inv_fact[n - m] % p;
}

// Lucas：C(n, m) % p
ll Lucas(ll n, ll m, ll p) {
    if (m == 0) return 1;
    return Lucas(n / p, m / p, p) * C_small(n % p, m % p, p) % p;
}

//=================扩展===================//

// 定义n[i]: n在p进制下第i位的值
// 定义m[i]: m在p进制下第i位的值
// 定义r: r = max(n的最高位, m的最高位), p进制下
// 位数基底是0
// C(n, m) % p = C(n[0], m[0]) * C(n[1], m[1]) * C(n[2], m[2]) * ... * C(n[r], m[r]) % p

//=============当p=2时的特例===============//

// C(n, m) % 2 = 1(奇数)
// 每一位的C(n[i], m[i]) = 1
// n[i] = 1时, m[i] = 1/0
// n[i] = 0时, m[i] = 0
// 即满足: (m & n) = m, m[i]的集合(即m)是n的二进制子集