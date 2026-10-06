#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using ill = __int128_t;
const int inf = 1e9;
const ll INF = 1e18;
const ll mod = 998244353;
const int MAXN = 3e5;

// 错排数 D[n] : 1..n 的排列, 没有任何元素留在原位的方案数
// D[0] = 1

// 常用的主要是第一个递推公式

// 递推(最常用和常考): D[0] = 1, D[1] = 0, D[n] = (n - 1) * (D[n - 1] + D[n - 2])
// 固定元素 x, 它放到位置 y (y != x), 共 n-1 种选择:
//   情况1: y 也放到 x 的位置  ->  剩下 n-2 个元素错排 D[n-2]
//   情况2: y 不放到 x 的位置  ->  把 x 的原位重标为 y 的禁位, 等价于 n-1 个元素错排 D[n-1]

// 递推：D[n] = n * D[n - 1] + (n - 1) * D[n - 2] = n * D[n - 1] + (-1)^n 

// 通项公式: 求和
// D[n] = for(int k :[0 -> n])
//          ans += pw(-1, k) * C(n, k) * (n - k)!

// 近似值求解:
// ======注意======
// 使用该公式求解理论上的答案是绝对正确的
// 导致误差错误的是：浮点数的运算和e的截断问题
// 如果只进行分数运算而不进行除法, 则不存在误差
// D(n) = round(n! / e)

// 扩展：
// 恰好有k个留在原位置R(n, k) = C(n, k) * D[n - k]

int D[MAXN + 1];

int pw(int a, int b) {
    int tmp = b;
    b = abs(b);
    int res = 1;
    while (b) {
        if (b & 1)res = (ill)res * (ill)a % mod;
        a = (ill)a * (ill)a % mod;
        b >>= 1;
    }
    if (tmp < 0) {
        return pw(res, mod - 2);
    }
    return res;
}

int inv(int x) {
    return pw(x, mod - 2);
}

// 略...
ll C(int n, int m) {}
ll fac(int n) {}

// 错排的线性递推
void f1() {
    D[0] = 1 % mod;
    D[1] = 0;
    for (int i = 2; i <= MAXN; i++) {
        D[i] = (ll)(i - 1) * ((D[i - 1] + D[i - 2]) % mod) % mod;
    }
}

// 通项公式求解
ll f2(int n) {
    ll ans = 0;
    for (int k = 0;k <= n;k++) {
        ans += pw(-1, k) * C(n, k) * fac(n - k);
    }
    return ans;
}

// 恰好有k个留在原位置
ll R(int n, int k) {
    return C(n, k) * D[n - k];
}

const ld E = expl(1.0L);
const ld PI = acosl(-1.0L);

// expl: 指数函数e的x次幂

// 方式1: round(n! / e), 用 lgamma 求 log(n!) 避免阶乘溢出, 四舍五入即精确值
ld D1Round(int n) {
    if (n == 0) return 1.0L;
    if (n == 1) return 0.0L;
    return roundl(expl(lgammal((ld)n + 1.0L) - 1.0L));
}


// 方式2: Stirling 近似, 直接算
ld D2Stirling(int n) {
    if (n == 0) return 1.0L;
    if (n == 1) return 0.0L;
    ld x = (ld)n;
    return sqrtl(2.0L * PI * x) * powl(x / E, x);
}



// 方式3: 对数形式(最稳, 不易溢出)
ld D3Log(int n) {
    if (n == 0) return 1.0L;
    if (n == 1) return 0.0L;
    ld x = (ld)n;
    ld logD = 0.5L * logl(2.0L * PI * x) + x * logl(x) - x;
    return expl(logD);
}