#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 组合恒等式：C(n, m) = C(n - 1, m) + C(n - 1, m - 1)
// C(n, m) = n!/ (m!*(n - m)!)
// 0 != 1, 0的0次幂 = 1

// 二项式系数的组合分解公式：
// C(j, i)* C(i, n) = C(j, n) * C(j - n, j - i)

const int mod = 1e9 + 7;
const int MAXN = 2e5;
ll fac[MAXN + 1];// fac[i]即(i!)对mod取模的结果
ll inv[MAXN + 1];// inv[i]即(i!)模mod意义下的逆元

ll pw(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1)res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

ll inv_(ll a) {
    return pw(a, mod - 2);
}

void init(int n) {
    fac[0] = 1;
    for (ll i = 1;i <= n;i++) {
        fac[i] = fac[i - 1] * i % mod;
    }

    inv[0] = 1;
    inv[n] = inv_(fac[n]);
    for (ll i = n - 1;i >= 1;i--) {
        inv[i] = (i + 1) * inv[i + 1] % mod;
    }
}

ll C(ll n, ll m) {
    return (((fac[n] * inv[m]) % mod) * inv[n - m]) % mod;
}


// 暴力求解组合数
ll C2(ll n, ll m) {
    if (m > n || m < 0) return 0;
    if (m == 0 || m == n) return 1;
    if (m > n - m) m = n - m;
    ll res = 1;
    for (ll i = 1; i <= m; i++) {
        res = res * (n - m + i) / i;
    }
    return res;
}
