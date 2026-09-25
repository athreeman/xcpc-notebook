#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using ill = __int128_t;

const int MAXN = 1e5;
const int mod = 1e9 + 7;

// 给定区间集合A, B
// 构造一个集合S(可以为空), 元素范围[1, n]
// 满足:
// 1. 任意区间A, 至少有一个元素存在
// 2. 任意区间B, 不能有元素存在
// 求解合法的构造方案数

// 枚举选择的最大元素i
// dp[i]: 表示选择i作为集合最大元素, 且右端点 <= i的区间的条件全部满足的方案数

// isEmpty: 是否存在合法的空集方案, 初始化1 
// 一旦遇到需要处理的合法区间, isEmpty就不可能为1, 之后isEmpty维持0

// total: 截止目前的合法方案数

// allowed: 元素i是否可以被选择
// diff: 差分数组, 辅助获取allowed数组

int n, m;
ll a[MAXN + 5];
int L[MAXN + 5];// L[i]表示右端点为i的最右左端点, 不存在初始化为-1
ll dp[MAXN + 5];
int diff[MAXN + 5];
int allowed[MAXN + 5];// allowed[i]：位置 i 是否可选

void solve() {
    cin >> n >> m;
    fill(L, L + n + 1, -1);
    fill(dp, dp + n + 1, 0);
    fill(diff, diff + n + 2, 0);

    // 至少存在一个元素
    for (int i = 1;i <= n;i++) {
        int l, r;
        cin >> l >> r;
        L[r] = max(L[r], l);
    }
    // 不能存在元素
    for (int i = 1;i <= m;i++) {
        int l, r;
        cin >> l >> r;
        diff[l]++;
        diff[r + 1]--;
    }

    for (int i = 1, cur = 0;i <= n;i++) {
        cur += diff[i];
        allowed[i] = (cur == 0) ? 1 : 0;
    }

    deque<array<ll, 2>>prev;
    ll total = 0, isEmpty = 1;
    for (int i = 1;i <= n;i++) {
        ll totalPrev = (total + isEmpty) % mod;

        // 对于不选i, L[i]>=0表示存在一个合法区间
        // 我们必须保证total的计数是j >= L[i]的dp[j], 否则视为非法方案

        if (L[i] >= 1) {
            isEmpty = 0;
            while (prev.size() && prev.front()[0] < L[i]) {
                total = (total - prev.front()[1] + mod) % mod;
                prev.pop_front();
            }
        }

        // 若可以选择i, i的方案数就是合法的dp[j]的和, 再加上可能存在的空集isEmpty选项
        // 因为选择了i, 所以对于前缀的dp[j], j<L[i]的同样合法

        if (allowed[i]) {
            dp[i] = totalPrev;

            // total累加
            total = (total + dp[i]) % mod;
            if (dp[i] > 0)prev.push_back({ i,dp[i] });
        }
    }
    cout << (total + isEmpty) % mod << '\n';
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
