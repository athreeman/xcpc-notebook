#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 勒让德公式: p必须是质因子
// 常数n, 质因子p
// 质因子p在n!中的指数(幂)
// vp(n!) = n/p + n/p^2 + n/p^3 + n/p^4 + ...(向下取整)

// 性质:
// sp(n): n的p进制下, 各位进制之和
// vp(p * n) = 1 + vp(n)
// vp(n) : 直接循环求解, 复杂度logp(n)
// 当且仅当, n % p != 0, vp(n) = 0
// vp(n!) = (n - sp(n)) / (p - 1) 
// vp((p * m)!) = m + vp(m!)

// vp(p * n) = 1 + vp(n)
// vp(a * b) = vp(a) + vp(b)
// vp(a / b) = vp(a) - vp(b)
// vp(a + b) >= min(vp(a), vp(b))
// vp(a - b) >= min(vp(a), vp(b))

//========================================//

// 如下给出, 当p = 2时的情况：
// 给定常数n > 0
// 定义k：n中的2次幂，即n质因子拆分后2的个数
// 定义m：非负整数，即m >= 0
// 定义v2(A)：求解给定常数A时，k的值
// 定义s(A)：数值A的二进制，1的个数
// 则存在
// v2(n!) = n / 2 + n / 4 + n / 8 + n / 16 + ...(向下取整)
// v2(n!) = n - s(n)

// v2((2 * m)!) = v2((2 * m + 1)!) = m + v2(m !)
// v2(n)：求解直接循环除2即可，复杂度logn
// 当且仅当n & 1，v2(n) = 0

// v2(n)的运算规律
// v2(2 * n) = 1 + v2(n)
// v2(a * b) = v2(a) + v2(b)
// v2(a / b) = v2(a) - v2(b)

// (当v2(a) != v2(b)时取等号，若v2(a) = v2(b)取严格大于)
// v2(a + b) >= min(v2(a), v2(b))
// v2(a - b) >= min(v2(a), v2(b))



// sp(n): n 在 p 进制下各位数字之和
ll sp(ll n, ll p) {
    ll sum = 0;
    while (n) {
        sum += n % p;
        n /= p;
    }
    return sum;
}


// vp(n): n 中质因子 p 的指数（个数）
// 复杂度 O(log_p n)
// 注意: vp(0) 无定义, 假设 n > 0
ll vp(ll n, ll p) {
    ll k = 0;
    while (n % p == 0) {
        n /= p;
        k++;
    }
    return k;
}


// vp(n!): n! 中质因子 p 的指数（勒让德公式）
// = floor(n/p) + floor(n/p^2) + floor(n/p^3) + ...
ll vp_fac(ll n, ll p) {
    ll ans = 0;
    while (n) {
        n /= p;
        ans += n;// 每轮累加 floor(n/p^i)
    }
    return ans;
}