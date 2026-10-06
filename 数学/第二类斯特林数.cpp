#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 给定n个元素
// 求解, 把这n个元素分为k个非空、无序集合的方案数
// 等价: n个不同的球放入k个相同且非空的盒子
// 等价: n元素集合到k元素集合的满射个数 = k! * S(n, k)

// 常见性质(推论):
// S(n, n) = 1
// S(n, 1) = 1
// S(n, 0) = 0, (n > 0)
// S(0, 0) = 1
// S(n, n - 1) = C(n, 2)
// S(n, n - 2) = C(n, 3) + 3 * C(n, 4)
// Bell数: B(n) = S(n, 0) + S(n, 1) + ... + S(n, n), (n个元素的全部划分总数)

// 定义下降幂 D(x, n): x * (x - 1) * (x - 2) * ... * (x - n + 1)
// 下降幂展开: x^n = S(n,0)*D(x,0) + S(n,1)*D(x,1) + ... + S(n,n)*D(x,n)

// 递推公式
// S(n, k) = S(n - 1, k - 1) + k * S(n - 1, k)

// 通项公式
// S(n, k) = (1 / k!) * ( Σ_{i=0}^{k} (-1)^i * C(k, i) * (k - i)^n )


// 和模2的关系(奇偶性)
// 当(n - m)& floor((m - 1) / 2)时, C(n, m) % 2 = 1


const int MAXN = 1005;
ll S[MAXN][MAXN];

// 以下皆为递推公式实现

// 迭代
void S1(int n) {
    S[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        S[i][0] = 0;
        for (int k = 1; k <= i; k++) {
            S[i][k] = S[i - 1][k - 1] + k * S[i - 1][k];
        }
    }
}


// 递归
ll dp[MAXN][MAXN];// dp需要手动初始化-1
ll S2(int n, int k) {
    if (k == 0) return (n == 0);
    if (n < k)  return 0;
    if (n == k) return 1;
    if (dp[n][k] != -1) return dp[n][k];
    return dp[n][k] = S2(n - 1, k - 1) + k * S2(n - 1, k);
}