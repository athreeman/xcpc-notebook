#include<bits/stdc++.h>
using namespace std;
using ld = long double;

ld fac[200000];// 阶乘


// 级数求解
ld E1(int K) {// K: 截断
    ld e = 0;
    for (int k = 0;k <= K;k++) {
        e += 1 / fac[k];
    }
    return e;
}

// 反向求和

// e = 1 + 1/1 * (1 + 1/2 * (1 + 1/3 * (1 + ... * (1 + 1/K) ... )))
//
//     E(K,k) = 1 + E(K,k+1) / (k+1)      ,  k < K
//     E(K,K) = 1
//     e ~= E(K,1)
//
// K=18 时: e = 2.7182818284590451

// 迭代
ld E2(int K) {
    ld e = 1;
    for (ld k = K; k >= 1; k--) {
        e = 1 + e / k;
    }
    return e;
}

// 递归
ld E3(int K, int k = 1) {
    if (k == K) return 1;              // 最内层
    return 1 + E3(K, k + 1) / (k + 1);  // 往外一层
}