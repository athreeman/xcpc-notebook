#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using ill = __int128_t;
const ll INF = 1e18;
const ll mod = 1e9 + 7;

// 鸽笼原理:
// 给定n个元素, 把他们分为k组
// 则满足, 必然存在至少一组的元素个数>=ceil(n/k)
// 扩展: 若分为k组, 保证存在一组至少有r个, 则n>k(r-1)
// 例题: https://codeforces.com/contest/2120/problem/D


// 思路:
// 由于题目要求(n,m)字典序最小, 我们优先考量行n的最小值
// 1、先考量行如何处理
// n行, 分配k个元素, 要让某个元素至少存现a列
// 通过二分查找最小的n满足ceil(n/k)>=a
// 2、考量固定行下的列
// 因素1：我们要让所有k的元素都出现过a行, 需要k列
// 因素2：我们要让某一个元素x的所有a行的分配可能都出现过, 需要C(n,a)列
// 因素3：我们要让某个元素的某个a行分配方式出现b次, 我们要首先让所有元素出现b-1次
// 因素4：最后的b次, 我们只需要再加1列即可
// 由于任何元素的任何a行分配都已出现过, 这一列必然可以成为某个分配的b列

ill a, b, k;

ill pw(ill x, ill y) {
    ill res = 1;
    while (y) {
        if (y & 1)res = res * x % mod;
        x = x * x % mod;
        y >>= 1;
    }
    return res;
}

ill inv(ill x) {
    return pw(x, mod - 2) % mod;
}

ill C(ill n, ill m) {
    if (m > n || m < 0) return 0;
    if (m == 0 || m == n) return 1;
    m = min(m, n - m);
    ill res = 1;
    for (ill i = 1; i <= m; i++) {
        res = res * (n - m + i) % mod * inv(i) % mod;
    }
    return res;
}

void solve() {
    ll in1, in2, in3;
    cin >> in1 >> in2 >> in3;
    a = in1, b = in2, k = in3;

    ill n, m;
    ill lo = a, hi = INF;
    while (lo <= hi) {
        ill mid = (lo + hi) >> 1;
        if ((mid + k - 1) / k >= a) {
            n = mid;
            hi = mid - 1;
        }
        else {
            lo = mid + 1;
        }
    }
    n = n % mod;
    m = ((b - 1) * k % mod * C(n, a) % mod + 1) % mod;
    cout << (ll)n << ' ' << (ll)m << '\n';

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}